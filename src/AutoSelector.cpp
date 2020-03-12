#include "User/AutoSelector.h"

AutoSelector::AutoSelector(std::string autoName, int defaultAutoNumber) {
  // Append default values to the vectors first
  this->autoNames.push_back(autoName);
  this->autoNumbers.push_back(defaultAutoNumber);
}

void AutoSelector::registerAuto(std::string autoName, int autoNumber) {
  // Append new auto routines to the vectors
  autoNames.push_back(autoName);
  autoNumbers.push_back(autoNumber);
}

void AutoSelector::listOptions() {
  // Create the dropdown
  autoList = createDropdown(lv_scr_act(), 150, 0, 100, 245, 2);
  // Create a string containing all of the routine autoNames

  std::string options = "";
  for(int i = 0; i < autoNames.size(); i++) {
    if(i == autoNames.size() - 1) {
      options += autoNames[i];
    } else {
      options += autoNames[i] + "\n";
    }
  }
  // Set the options to the dropdown
  lv_ddlist_set_options(autoList, options.c_str());
}

int AutoSelector::getSelectedAuto() {
  // By using the selected dropdown as the index, we cover
  // the case where the numbers are not in order
  return autoNumbers[lv_ddlist_get_selected(autoList)];
}


lv_obj_t * AutoSelector::createDropdown(lv_obj_t * parent, lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height,
    int id)
{
    lv_obj_t * dd = lv_ddlist_create(parent, NULL);
    lv_obj_set_pos(dd, x, y);
    lv_obj_set_size(dd, width, height-50);
    lv_ddlist_set_fix_height(dd, height);
    lv_obj_set_free_num(dd, id);
    lv_ddlist_set_sb_mode(dd, LV_SB_MODE_ON);
    return dd;
}

AutoSelector::~AutoSelector() {
  delete autoList;
}
