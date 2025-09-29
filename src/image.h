#ifndef COMPUTER_VISION_PROJECTS_IMAGE_H
#define COMPUTER_VISION_PROJECTS_IMAGE_H

#include <cstdint>
#include <string>
#include <vector>

namespace ComputerVisionProjects {

class Image {
 public:
  Image() = default;
  Image(int rows, int cols, int max_val = 255);

  bool ReadFromPGM(const std::string &filename);
  bool WriteToPGM(const std::string &filename) const;

  int num_rows() const { return rows_; }
  int num_columns() const { return cols_; }
  int max_val() const { return max_val_; }

  uint8_t GetPixel(int r, int c) const;
  void SetPixel(int r, int c, uint8_t value);

 private:
  int rows_ = 0;
  int cols_ = 0;
  int max_val_ = 255;
  std::vector<uint8_t> data_;
};

}  // namespace ComputerVisionProjects

#endif  // COMPUTER_VISION_PROJECTS_IMAGE_H