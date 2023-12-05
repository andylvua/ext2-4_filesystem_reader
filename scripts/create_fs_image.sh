if [ $# -ne 3 ]; then
    echo "Usage: $0 <image name> <image size> <filesystem type>"
    exit 1
fi

if [ -z "$1" ]; then
    echo "Error: image name cannot be empty"
    exit 1
fi
if ! [[ "$2" =~ ^[0-9]+$ ]]; then
    echo "Error: image size must be a positive integer"
    exit 1
fi
if [ "$3" != "ext2" ] && [ "$3" != "ext3" ] && [ "$3" != "ext4" ]; then
    echo "Error: filesystem type must be ext2, ext3, or ext4"
    exit 1
fi

image_name="$1"

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

echo "The image will be created with the following parameters:"
echo "Image name: $image_name"
echo "Image size: $2 MB"
echo -e "Filesystem type: $3\n"

read -r -p "Continue? [Y/n] " response
case "$response" in
    [yY][eE][sS]|[yY]|"")
        ;;
    *)
        echo "Aborting"
        exit 1
        ;;
esac

dd if=/dev/zero of="$image_name" bs=1MB count="$2"
if [ $? -ne 0 ]; then
    echo "Error: failed to create image"
    exit 1
fi

mkfs -t "$3" "$image_name"
if [ $? -ne 0 ]; then
    echo "Error: failed to create filesystem"
    exit 1
fi

echo "Image created successfully."

read -r -p "Mount image? [Y/n] " response
case "$response" in
    [yY][eE][sS]|[yY]|"")
        ;;
    *)
        echo "Aborting"
        exit 0
        ;;
esac

mkdir -p /mnt/"$image_name"
if [ $? -ne 0 ]; then
    echo "Error: failed to create mount point"
    exit 1
fi

mount -o loop "$image_name" /mnt/"$image_name"
if [ $? -ne 0 ]; then
    echo "Error: failed to mount image"
    exit 1
fi

echo "Image mounted successfully. You can access it at /mnt/$image_name"

exit 0
