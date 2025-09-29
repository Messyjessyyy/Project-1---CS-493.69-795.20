#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "image.h"
#include "DisjSets.h"

using namespace ComputerVisionProjects;

static bool IsForeground(uint8_t v) {
  return v > 0; // binary: non-zero is foreground
}

int main(int argc, char **argv){
  if (argc!=3) {
    std::cout << "Usage: " << argv[0] << " {input_image_name} {output_image_name}" << std::endl;
    return 1;
  }
  const std::string input_file(argv[1]);
  const std::string output_file(argv[2]);

  Image img;
  if (!img.ReadFromPGM(input_file)) {
    std::cerr << "Error: cannot read image: " << input_file << std::endl;
    return 2;
  }

  const int rows = img.num_rows();
  const int cols = img.num_columns();

  std::vector<int> labels(rows * cols, 0);
  int next_label = 1;
  DisjSets uf(rows * cols + 1);

  auto index_of = [cols](int r, int c) { return r * cols + c; };

  // First pass (8-connectivity): assign provisional labels and record equivalences
  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {
      if (!IsForeground(img.GetPixel(r, c))) continue;

      int neighbors[4][2] = { {r, c-1}, {r-1, c-1}, {r-1, c}, {r-1, c+1} };
      int neighbor_labels[4];
      int count = 0;
      for (auto &nb : neighbors) {
        int nr = nb[0];
        int nc = nb[1];
        if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && IsForeground(img.GetPixel(nr, nc))) {
          int lbl = labels[index_of(nr, nc)];
          if (lbl > 0) neighbor_labels[count++] = lbl;
        }
      }

      if (count == 0) {
        labels[index_of(r, c)] = next_label;
        next_label++;
      } else {
        int min_lbl = neighbor_labels[0];
        for (int i = 1; i < count; ++i) if (neighbor_labels[i] < min_lbl) min_lbl = neighbor_labels[i];
        labels[index_of(r, c)] = min_lbl;
        for (int i = 0; i < count; ++i) uf.Union(min_lbl, neighbor_labels[i]);
      }
    }
  }

  // Second pass: resolve labels to roots and remap to compact sequential labels
  std::unordered_map<int,int> root_to_compact;
  int compact = 1;
  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {
      int &lbl = labels[index_of(r, c)];
      if (lbl == 0) continue;
      int root = uf.Find(lbl);
      auto it = root_to_compact.find(root);
      if (it == root_to_compact.end()) {
        root_to_compact[root] = compact++;
        lbl = root_to_compact[root];
      } else {
        lbl = it->second;
      }
    }
  }

  // Paint labeled image with distinct gray levels
  int num_components = compact - 1;
  if (num_components == 0) {
    // Write out blank image
    Image out(rows, cols, 255);
    for (int r = 0; r < rows; ++r)
      for (int c = 0; c < cols; ++c)
        out.SetPixel(r, c, 0);
    if (!out.WriteToPGM(output_file)) {
      std::cerr << "Error: cannot write output: " << output_file << std::endl;
      return 3;
    }
    return 0;
  }

  Image out(rows, cols, 255);
  // Distribute labels over 1..255 range. Ensure background is 0.
  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {
      int lbl = labels[index_of(r, c)];
      if (lbl == 0) { out.SetPixel(r, c, 0); continue; }
      // Map compact label in [1..num_components] to gray in [1..255]
      uint8_t gray = static_cast<uint8_t>( (lbl * 255) / (num_components + 1) );
      if (gray == 0) gray = 1;
      out.SetPixel(r, c, gray);
    }
  }

  if (!out.WriteToPGM(output_file)) {
    std::cerr << "Error: cannot write output: " << output_file << std::endl;
    return 3;
  }

  return 0;
}