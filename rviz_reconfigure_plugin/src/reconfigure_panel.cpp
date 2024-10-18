#include "rviz_dynamic_reconfigure_plugin/dynamic_reconfigure_panel.h"

#include <pluginlib/class_list_macros.h>
#include <ros/console.h>

namespace rviz_dynamic_reconfigure_plugin
{

DynamicReconfigurePanel::DynamicReconfigurePanel(QWidget* parent)
  : rviz::Panel(parent)
{
  QVBoxLayout* layout = new QVBoxLayout;

  node_name_editor_ = new QLineEdit;
  layout->addWidget(node_name_editor_);

  update_button_ = new QPushButton("Update Parameters");
  layout->addWidget(update_button_);

  param_tree_ = new QTreeWidget;
  param_tree_->setHeaderLabels(QStringList() << "Parameter" << "Value");
  layout->addWidget(param_tree_);

  setLayout(layout);

  connect(node_name_editor_, SIGNAL(editingFinished()), this, SLOT(updateNodeName()));
  connect(update_button_, SIGNAL(clicked()), this, SLOT(updateParams()));
}

void DynamicReconfigurePanel::load(const rviz::Config& config)
{
  rviz::Panel::load(config);
  QString node_name;
  if (config.mapGetString("NodeName", &node_name))
  {
    node_name_editor_->setText(node_name);
    updateNodeName();
  }
}

void DynamicReconfigurePanel::save(rviz::Config config) const
{
  rviz::Panel::save(config);
  config.mapSetValue("NodeName", node_name_editor_->text());
}

void DynamicReconfigurePanel::updateNodeName()
{
  node_name_ = node_name_editor_->text().toStdString();
  client_.reset(new dynamic_reconfigure::Client<dynamic_reconfigure::Config>(node_name_));
}

void DynamicReconfigurePanel::updateParams()
{
  if (!client_)
  {
    ROS_ERROR("Client not initialized. Please enter a valid node name.");
    return;
  }

  dynamic_reconfigure::Config config;
  if (!client_->getCurrentConfiguration(config))
  {
    ROS_ERROR("Failed to get current configuration for node %s", node_name_.c_str());
    return;
  }

  displayParams(config);
}

void DynamicReconfigurePanel::displayParams(const dynamic_reconfigure::Config& config)
{
  param_tree_->clear();

  for (const auto& param : config.bools)
  {
    QTreeWidgetItem* item = new QTreeWidgetItem(param_tree_);
    item->setText(0, QString::fromStdString(param.name));
    item->setText(1, param.value ? "true" : "false");
  }

  for (const auto& param : config.ints)
  {
    QTreeWidgetItem* item = new QTreeWidgetItem(param_tree_);
    item->setText(0, QString::fromStdString(param.name));
    item->setText(1, QString::number(param.value));
  }

  for (const auto& param : config.doubles)
  {
    QTreeWidgetItem* item = new QTreeWidgetItem(param_tree_);
    item->setText(0, QString::fromStdString(param.name));
    item->setText(1, QString::number(param.value));
  }

  for (const auto& param : config.strs)
  {
    QTreeWidgetItem* item = new QTreeWidgetItem(param_tree_);
    item->setText(0, QString::fromStdString(param.name));
    item->setText(1, QString::fromStdString(param.value));
  }
}

} // end namespace rviz_dynamic_reconfigure_plugin

PLUGINLIB_EXPORT_CLASS(rviz_dynamic_reconfigure_plugin::DynamicReconfigurePanel, rviz::Panel)
