//
// Created by andrew on 11/24/23.
//

#ifndef FS_READER_FLAG_MAPPINGS_H
#define FS_READER_FLAG_MAPPINGS_H

#include <map>
#include <string>
#include <cstdint>

using flags_map_decltype = std::map <uint32_t, std::string>;


extern const flags_map_decltype state_map;

extern const flags_map_decltype err_behavior_map;

extern const flags_map_decltype os_map;

extern const flags_map_decltype compat_feature_map;
extern const flags_map_decltype ro_compat_feature_map;
extern const flags_map_decltype incompat_feature_map;

extern const flags_map_decltype misc_flags_map;

extern const flags_map_decltype default_mount_opts_map;

extern const flags_map_decltype journal_mode_map;

extern const flags_map_decltype file_type_map;

extern const flags_map_decltype inode_flags_map;

extern const flags_map_decltype partition_types_map;

extern const flags_map_decltype jbd2_compat_feature_map;
extern const flags_map_decltype jbd2_incompat_feature_map;
extern const flags_map_decltype jbd2_ro_compat_feature_map;

#endif //FS_READER_FLAG_MAPPINGS_H
