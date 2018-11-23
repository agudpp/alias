#ifndef TAG_SELECTION_HELPER_H
#define TAG_SELECTION_HELPER_H

#include <vector>

#include <ui_client/tag/tag_widget.h>

class TagSelectionHelper {
  public:
    TagSelectionHelper(void) :
      index_(-1)
    {}

    inline void
    setTags(const std::vector<TagWidget*>& tags)
    {
      unselect();
      elements_ = tags;
      index_ = -1;
    }

    inline void
    selectNext(void)
    {
      unselect();
      index_ = (index_ + 1) % elements_.size();
      select(index_);
    }

    inline void
    removeCurrent(void)
    {
      unselect();
      if (index_ >= 0 && index_ < elements_.size()) {
        elements_.erase(elements_.begin() + index_);
      }
      index_--;
      select(index_);
    }

    inline void
    selectPrev(void)
    {
      unselect();
      index_--;
      if (index_ < 0) {
        index_ = elements_.size() - 1;
      }
      select(index_);
    }

    inline bool
    hasSelection(void) const
    {
      return index_ >= 0 && index_ < elements_.size();
    }

    inline TagWidget*
    current(void)
    {
      return elements_[index_];
    }
    inline const TagWidget*
    current(void) const
    {
      return elements_[index_];
    }

    inline void
    select(int index)
    {
      index_ = index;
      if (index < elements_.size()) {
        elements_[index_]->highlight();
      }
    }

    inline void
    unselect(void)
    {
      if (index_ >= 0 && index_ < elements_.size()) {
        elements_[index_]->unhighlight();
      }
      index_ = -1;
    }


  private:
    std::vector<TagWidget*> elements_;
    int index_;
};

#endif // TAG_SELECTION_HELPER_H
