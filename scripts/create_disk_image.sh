#!/bin/bash

if [ $# -lt 4 ]; then
    echo "Usage: $0 <image name> <image size> <partition table type (MBR or GPT)> -p <partition size> <filesystem type> [-p ...] [-f <partition size>] ..."
    exit 1
fi

image_name="$1"
image_size="$2"
partition_table_type=$(echo "$3" | tr '[:lower:]' '[:upper:]')

if [ -z "$image_name" ]; then
    echo "Error: image name cannot be empty"
    exit 1
fi

if ! [[ "$image_size" =~ ^[0-9]+$ ]]; then
    echo "Error: image size must be a positive integer"
    exit 1
fi

if [ "$partition_table_type" != "MBR" ] && [ "$partition_table_type" != "GPT" ]; then
    echo "Error: partition table type must be MBR or GPT"
    exit 1
fi

shift 3

if [[ "$image_name" != *.img ]]; then
    image_name="$image_name.img"
fi

if [ -f "$image_name" ]; then
    echo "Error: image $image_name already exists. Do you want to overwrite it?"
    read -r -p "Continue? [Y/n] " response
    case "$response" in
        [yY][eE][sS]|[yY]|"")
            ;;
        *)
            echo "Aborting"
            exit 0
            ;;
    esac

    rm "$image_name"
    if [ $? -ne 0 ]; then
        echo "Error: failed to remove existing image"
        exit 1
    fi
fi

partitions=()
while [ $# -gt 0 ]; do
    case "$1" in
        -p)
            shift
            partition_size="$1"
            shift
            filesystem_type="$1"
            if [ "$filesystem_type" != "ext2" ] && [ "$filesystem_type" != "ext3" ] && [ "$filesystem_type" != "ext4" ]; then
                echo "Error: filesystem type must be ext2, ext3, or ext4"
                exit 1
            fi
            partitions+=("$partition_size,$filesystem_type")
            shift
            ;;
        -f)
            shift
            partition_size="$1"
            partitions+=("$partition_size,")
            shift
            ;;
        *)
            echo "Error: Invalid option $1"
            exit 1
            ;;
    esac
done

total_size=0
for partition in "${partitions[@]}"; do
    size=$(echo "$partition" | cut -d ',' -f 1)
    total_size=$(($total_size + $size))
done

if [ "$total_size" -gt "$image_size" ]; then
    echo "Error: total size of partitions is greater than image size"
    exit 1
fi

echo "The disk image will be created with the following parameters:"
echo "Image name: $image_name"
echo "Image size: $image_size MB"
echo "Partition table type: $partition_table_type"
echo "Partitions:"
for partition in "${partitions[@]}"; do
    size=$(echo "$partition" | cut -d ',' -f 1)
    filesystem=$(echo "$partition" | cut -d ',' -f 2)
    if [ -z "$filesystem" ]; then
        echo -e "\tFree space: $size MB"
    else
        echo -e "\tSize: $size MB. Filesystem: $filesystem"
    fi
done

read -r -p "Continue? [Y/n] " response
case "$response" in
    [yY][eE][sS]|[yY]|"")
        ;;
    *)
        echo "Aborting"
        exit 1
        ;;
esac

dd if=/dev/zero of="$image_name" bs=1MB count="$image_size"
if [ $? -ne 0 ]; then
    echo "Error: failed to create disk image"
    exit 1
fi

if [ "$partition_table_type" == "MBR" ]; then
    parted -s "$image_name" mklabel msdos
else
    parted -s "$image_name" mklabel gpt
fi

start_sector=1
for partition in "${partitions[@]}"; do
    size=$(echo "$partition" | cut -d ',' -f 1)
    filesystem=$(echo "$partition" | cut -d ',' -f 2)
    if [ -z "$filesystem" ]; then
        start_sector=$(($start_sector + $size))
        continue
    fi
    parted -s "$image_name" mkpart primary "$filesystem" "$start_sector" "$(($start_sector + $size))"

    if [ $? -ne 0 ]; then
        echo "Error: failed to create partition"
        exit 1
    fi
    start_sector=$(($start_sector + $size))
    if [ $? -ne 0 ]; then
        echo "Error: failed to create filesystem for partition"
        exit 1
    fi
done

dev="$(sudo losetup --show -f -P "$image_name")"
if [ $? -ne 0 ]; then
    echo "Error: failed to create loop device for disk image"
    exit 1
fi

mount_points=()
for part in "$dev"?*; do
   if [ "$part" = "${dev}p*" ]; then
       part="${dev}"
   fi
    dst="/mnt/$(basename "$part")"
    mount_points+=("$part $dst")
done

for ((i=0; i<${#mount_points[@]}; i++)); do
    mount_point="${mount_points[$i]}"
    part=$(echo "$mount_point" | cut -d ' ' -f 1)
    dst=$(echo "$mount_point" | cut -d ' ' -f 2)
    partition="${partitions[$i]}"
    filesystem=$(echo "$partition" | cut -d ',' -f 2)
    sudo mkfs -t "$filesystem" "$part"
    if [ $? -ne 0 ]; then
        echo "Error: failed to create filesystem for partition"
        exit 1
    fi
done

echo "Disk image created successfully."

read -r -p "Mount partitions? [Y/n] " response
case "$response" in
    [yY][eE][sS]|[yY]|"")
        ;;
    *)
        sudo losetup -d "$dev"
        echo "Partitions not mounted. You can mount them later manually."
        exit 0
        ;;
esac

for mount_point in "${mount_points[@]}"; do
    part=$(echo "$mount_point" | cut -d ' ' -f 1)
    dst=$(echo "$mount_point" | cut -d ' ' -f 2)
    sudo mkdir -p "$dst"
    sudo mount "$part" "$dst"
    if [ $? -ne 0 ]; then
        echo "Error: failed to mount partition"
        exit 1
    fi
done

echo "Partitions mounted successfully. Mount points:"
for mount_point in "${mount_points[@]}"; do
    dst=$(echo "$mount_point" | cut -d ' ' -f 2)
    echo -e "\t$dst"
done

exit 0
