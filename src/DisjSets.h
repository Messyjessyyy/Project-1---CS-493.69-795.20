#ifndef COMPUTER_VISION_PROJECTS_DISJSETS_H
#define COMPUTER_VISION_PROJECTS_DISJSETS_H

#include <vector>

namespace ComputerVisionProjects {

class DisjSets {
 public:
  explicit DisjSets(int num_elements) : parent_(num_elements), rank_(num_elements, 0) {
    for (int i = 0; i < num_elements; ++i) parent_[i] = i;
  }

  int Find(int x) {
    if (parent_[x] != x) parent_[x] = Find(parent_[x]);
    return parent_[x];
  }

  void Union(int a, int b) {
    int ra = Find(a);
    int rb = Find(b);
    if (ra == rb) return;
    if (rank_[ra] < rank_[rb]) {
      parent_[ra] = rb;
    } else if (rank_[ra] > rank_[rb]) {
      parent_[rb] = ra;
    } else {
      parent_[rb] = ra;
      rank_[ra]++;
    }
  }

  int Size() const { return static_cast<int>(parent_.size()); }

 private:
  std::vector<int> parent_;
  std::vector<int> rank_;
};

}  // namespace ComputerVisionProjects

#endif  // COMPUTER_VISION_PROJECTS_DISJSETS_H