#include "rviz_reconfigure_plugin/reconfigure_panel.h"

#include <QVBoxLayout>

namespace rviz_reconfigure_plugin {

ReconfigurePanel::ReconfigurePanel(QWidget* parent) : rviz::Panel(parent) {
  QVBoxLayout* layout = new QVBoxLayout;
  param_client_widget_ = new rqt_reconfigure::ParamClientWidget(this);
  layout->addWidget(param_client_widget_);
  setLayout(layout);
}

void ReconfigurePanel::load(const rviz::Config& config) {
  rviz::Panel::load(config);
}

void ReconfigurePanel::save(rviz::Config config) const {
  rviz::Panel::save(config);
}

} // end namespace rviz_reconfigure_plugin

#include <pluginlib/class_list_macros.h>
PLUGINLIB_EXPORT_CLASS(rviz_reconfigure_plugin::ReconfigurePanel, rviz::Panel)
