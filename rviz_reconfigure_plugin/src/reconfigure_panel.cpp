#include "rviz_reconfigure_plugin/reconfigure_panel.h"
#include <QVBoxLayout>
#include <pluginlib/class_list_macros.h>

namespace rviz_reconfigure_plugin {

ReconfigurePanel::ReconfigurePanel(QWidget* parent) : rviz::Panel(parent) {
  QVBoxLayout* layout = new QVBoxLayout;
  param_widget_ = new ParamWidget;
  layout->addWidget(param_widget_);
  setLayout(layout);
}

ReconfigurePanel::~ReconfigurePanel() {
  delete param_widget_;
}

void ReconfigurePanel::load(const rviz::Config& config) {
  rviz::Panel::load(config);
  // Load any additional configuration if needed
}

void ReconfigurePanel::save(rviz::Config config) const {
  rviz::Panel::save(config);
  // Save any additional configuration if needed
}

} // end namespace rviz_reconfigure_plugin

PLUGINLIB_EXPORT_CLASS(rviz_reconfigure_plugin::ReconfigurePanel, rviz::Panel)
