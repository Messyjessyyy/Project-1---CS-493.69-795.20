#include "image.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace ComputerVisionProjects {

Image::Image(int rows, int cols, int max_val) : rows_(rows), cols_(cols), max_val_(max_val), data_(rows * cols, 0) {}

static void SkipComments(std::istream &in) {
  while (in >> std::ws && in.peek() == '#') {
    std::string dummy;
    std::getline(in, dummy);
  }
}

bool Image::ReadFromPGM(const std::string &filename) {
  std::ifstream in(filename, std::ios::binary);
  if (!in) return false;

  std::string magic;
  in >> magic;
  if (magic != "P5" && magic != "P2") return false;

  SkipComments(in);
  in >> cols_;
  SkipComments(in);
  in >> rows_;
  SkipComments(in);
  in >> max_val_;
  if (!in) return false;

  if (max_val_ <= 0 || max_val_ > 65535) return false;

  data_.assign(rows_ * cols_, 0);

  if (magic == "P5") {
    in.get();
    if (max_val_ < 256) {
      in.read(reinterpret_cast<char*>(data_.data()), static_cast<std::streamsize>(data_.size()));
      return static_cast<bool>(in);
    } else {
      for (int i = 0; i < rows_ * cols_; ++i) {
        unsigned char bytes[2];
        in.read(reinterpret_cast<char*>(bytes), 2);
        if (!in) return false;
        uint16_t val = static_cast<uint16_t>(bytes[0]) << 8 | static_cast<uint16_t>(bytes[1]);
        data_[i] = static_cast<uint8_t>((val * 255) / max_val_);
      }
      max_val_ = 255;
      return true;
    }
  } else {
    for (int i = 0; i < rows_ * cols_; ++i) {
      int v;
      SkipComments(in);
      in >> v;
      if (!in) return false;
      if (v < 0) v = 0;
      if (v > max_val_) v = max_val_;
      data_[i] = static_cast<uint8_t>((v * 255) / max_val_);
    }
    max_val_ = 255;
    return true;
  }
}

bool Image::WriteToPGM(const std::string &filename) const {
  std::ofstream out(filename, std::ios::binary);
  if (!out) return false;
  out << "P5\n" << cols_ << " " << rows_ << "\n" << 255 << "\n";
  out.write(reinterpret_cast<const char*>(data_.data()), static_cast<std::streamsize>(data_.size()));
  return static_cast<bool>(out);
}

uint8_t Image::GetPixel(int r, int c) const {
  if (r < 0 || r >= rows_ || c < 0 || c >= cols_) return 0;
  return data_[r * cols_ + c];
}

void Image::SetPixel(int r, int c, uint8_t value) {
  if (r < 0 || r >= rows_ || c < 0 || c >= cols_) return;
  data_[r * cols_ + c] = value;
}

}  // namespace ComputerVisionProjects