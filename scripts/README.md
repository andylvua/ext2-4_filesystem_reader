# Image helper scripts

In this directory you can find some helper scripts for creating image files. This allows you to create
custom test images with your own data or to create images with a specific partition table and filesystems.

We hope that this will help you to check this already extensive lab.

> **Note**
> 
> The ready-made images are not included in this repository for the sake of size.
> However, if you want to use ready-made images, you can download them from
> [this repository](https://drive.google.com/drive/folders/1fgvzQ7_YzfaE-RZhHIiaR5RYWQuV9f88?usp=sharing)

> **Warning**
> 
> These scripts are intended to be run as root.
> Please keep it in mind as some features may not work properly if you run them without root privileges.


## `create_fs_image.sh`

This script creates an empty image file with a filesystem on it. 

### Usage

```
chmod +x create_fs_image.sh
sudo ./create_fs_image.sh <image_file> <filesystem_type> <filesystem_size>
```

Where:
* `<image_file>` is the name of the image file to create
* `<filesystem_type>` is the type of the filesystem to create. Supported filesystems are:
  * `ext2`
  * `ext3`
  * `ext4`
* `<filesystem_size>` is the size of the filesystem in megabytes

### Example

```
./create_fs_image.sh test.img ext2 100
```

This will create a file named `test.img` with an ext2 filesystem of 100MB.

The script will guide you through the process of creating the filesystem. In the end it will also
ask you if you want to mount the filesystem.

## `create_disk_image.sh`

This script creates an empty image file with a specified partition table and partitions on it.

### Usage

```
chmod +x create_disk_image.sh
sudo ./create_disk_image.sh <image_file> <image_size> <partition_table_type> -p <partition_size> <filesystem_type> [-p ...] [-f <partition_size>] [-f ...]
```

Where:
* `<image_file>` is the name of the image file to create
* `<image_size>` is the size of the image file in megabytes
* `<partition_table_type>` is the type of the partition table to create. Supported partition tables are:
  * `MBR`
  * `GPT`

The script takes arbitrary number of `-p` and `-f` options.

* `-p` specifies a partition. It takes two arguments:
  * `<partition_size>` is the size of the partition in megabytes
  * `<filesystem_type>` is the type of the filesystem to create on the partition. Supported filesystems are:
    * `ext2`
    * `ext3`
    * `ext4`
* `-f` specifies a free space. It takes one argument:
  * `<partition_size>` is the size of the free space in megabytes

Options are processed in the order they are specified. 

You should specify at least one partition with the `-p` option. If you don't specify any free space with the `-f` option,
the partitions will be created contiguously. Any free space
after the last partition will be ignored.

### Example

```
./create_disk_image.sh test.img 100 MBR -p 10 ext2 -p 20 ext3 -p 30 ext4
```

This will create a file named `test.img` with an MBR partition table and three partitions on it:
* 10MB ext2 partition
* 20MB ext3 partition
* 30MB ext4 partition

The script will guide you through the process of creating the filesystems. 

Similarly to `create_fs_image.sh`,
it will also ask you if you want to mount the partitions. If you choose to,
they will be automatically mounted to `/mnt/loop<n>p<m>` where `n` is the number of the loop device and `m` is the number
of the partition. The mount points will be printed to the standard output.
