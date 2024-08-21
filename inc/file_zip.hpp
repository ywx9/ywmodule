/// \file file_zip.hpp
/// \brief to handle zip files

#pragma once

#ifndef YWLIB
#include <algorithm>
#include <iterator>
#include <set>
#else
import std;
#endif

#include "array.hpp"
#include "logger.hpp"
#include "file.hpp"
#include "string.hpp"


export namespace yw::file::zip {


/// central directory file header
struct CentralDirectoryFileHeader {
  nat4 signature;
  nat2 version_made_by;
  nat2 version_needed_to_extract;
  nat2 general_purpose_bit_flag;
  nat2 compression_method;
  nat2 last_mod_file_time;
  nat2 last_mod_file_date;
  nat4 crc32;
  nat4 compressed_size;
  nat4 uncompressed_size;
  nat2 file_name_length;
  nat2 extra_field_length;
  nat2 file_comment_length;
  nat2 disk_number_start;
  nat2 internal_file_attributes;
  nat4 external_file_attributes;
  nat4 relative_offset_of_local_header;
  cat1 text[];

  stv1 file_name() const noexcept {
    return stv1(text, file_name_length);
  }
  stv1 extra_field() const noexcept {
    return stv1(text + file_name_length, extra_field_length);
  }
  stv1 file_comment() const noexcept {
    return stv1(text + file_name_length + extra_field_length, file_comment_length);
  }

  friend bool operator<(
    const CentralDirectoryFileHeader& a,
    const CentralDirectoryFileHeader& b) {
    return a.file_name() < b.file_name();
  }
};


/// end of central directory record
struct EndOfCentralDirectoryRecord {
  nat4 signature;
  nat2 number_of_this_disk;
  nat2 number_of_disk_with_start_of_central_directory;
  nat2 total_number_of_entries_in_the_central_directory_on_this_disk;
  nat2 total_number_of_entries_in_the_central_directory;
  nat4 size_of_the_central_directory;
  nat4 offset_of_start_of_central_directory;
  nat2 comment_length;
  cat1 text[];

  stv1 comment() const noexcept {
    return stv1(text, comment_length);
  }
};


} ////////////////////////////////////////////////////////////////////////////// namespace yw::file::zip


namespace yw::_ {


inline nat8 _zip_search_eocd(const Array<cat1>& file_data) {
  auto find_end_result = std::ranges::find_end(
    file_data, Array{0x50, 0x4b, 0x05, 0x06}).begin();
  if (find_end_result == file_data.end()) {
    logger.error("End of central directory signature not found.");
    return 0;
  }
  return nat8(std::ranges::distance(file_data.begin(), find_end_result));
}


inline std::set<file::zip::CentralDirectoryFileHeader>
_zip_enum_file(const Array<cat1>& file_data) {
  auto eocd_offset = _zip_search_eocd(file_data);
  if (eocd_offset == 0) return {};
  auto* eocd = (file::zip::EndOfCentralDirectoryRecord*)(file_data.data() + eocd_offset);
  if (eocd->total_number_of_entries_in_the_central_directory == 0) return {};
  else if (eocd->size_of_the_central_directory == 0) return {};
  else if (eocd->size_of_the_central_directory == 0xFFFFFFFF) {
    logger.error("Zip64 format is not supported.");
    return {};
  }
  std::set<file::zip::CentralDirectoryFileHeader> central_directory;

  for (nat i{}; i < eocd->total_number_of_entries_in_the_central_directory; ++i) {
    auto* cd = (file::zip::CentralDirectoryFileHeader*)(file_data.data() + eocd->offset_of_start_of_central_directory + i * sizeof(file::zip::CentralDirectoryFileHeader));
    central_directory.insert(*cd);
  }


}


}


export namespace yw::file::zip {


/// zip file properties
class Property {
public:
  nat8 central_directory_offset;

  std::set<CentralDirectoryFileHeader> central_directory;
};

}

namespace yw::_ {



}


export namespace yw::file {




}
