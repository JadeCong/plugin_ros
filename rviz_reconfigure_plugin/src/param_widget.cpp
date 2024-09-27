#include "rviz_reconfigure_plugin/param_widget.h"
#include <QVBoxLayout>
#include <QHeaderView>

namespace rviz_reconfigure_plugin {

ParamWidget::ParamWidget(QWidget* parent) : QWidget(parent) {
  QVBoxLayout* layout = new QVBoxLayout;

  node_name_edit_ = new QLineEdit;
  layout->addWidget(node_name_edit_);

  update_button_ = new QPushButton("Update Parameters");
  layout->addWidget(update_button_);

  param_tree_ = new QTreeWidget;
  param_tree_->setHeaderLabels(QStringList() << "Parameter" << "Value");
  param_tree_->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
  layout->addWidget(param_tree_);

  setLayout(layout);

  connect(update_button_, SIGNAL(clicked()), this, SLOT(updateParams()));
  connect(param_tree_, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(onItemChanged(QTreeWidgetItem*, int)));
}

void ParamWidget::updateParams() {
  std::string node_name = node_name_edit_->text().toStdString();
  
  client_.reset(new dynamic_reconfigure::Client<dynamic_reconfigure::Config>(node_name));

  if (client_->isConnected()) {
    dynamic_reconfigure::Config config;
    client_->getConfiguration(config);
    updateParamTree(config);
  }
  else {
    ROS_ERROR_STREAM("Failed to connect to node " << node_name);
  }
}

void ParamWidget::updateParamTree(const dynamic_reconfigure::Config& config) {
  param_tree_->clear();
  param_items_.clear();

  for (const auto& param : config.bools) {
    QTreeWidgetItem* item = new QTreeWidgetItem(param_tree_);
    item->setText(0, QString::fromStdString(param.name));
    item->setCheckState(1, param.value ? Qt::Checked : Qt::Unchecked);
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    param_items_[param.name] = item;
  }

  for (const auto& param : config.ints) {
    QTreeWidgetItem* item = new QTreeWidgetItem(param_tree_);
    item->setText(0, QString::fromStdString(param.name));
    item->setText(1, QString::number(param.value));
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    param_items_[param.name] = item;
  }

  for (const auto& param : config.doubles) {
    QTreeWidgetItem* item = new QTreeWidgetItem(param_tree_);
    item->setText(0, QString::fromStdString(param.name));
    item->setText(1, QString::number(param.value));
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    param_items_[param.name] = item;
  }

  for (const auto& param : config.strs) {
    QTreeWidgetItem* item = new QTreeWidgetItem(param_tree_);
    item->setText(0, QString::fromStdString(param.name));
    item->setText(1, QString::fromStdString(param.value));
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    param_items_[param.name] = item;
  }
}

void ParamWidget::onItemChanged(QTreeWidgetItem* item, int column) {
  if (column != 1 || !client_) return;

  std::string name = item->text(0).toStdString();
  std::string value = item->text(1).toStdString();

  dynamic_reconfigure::Config conf;

  if (item->checkState(1) != Qt::PartiallyChecked) {
    dynamic_reconfigure::BoolParameter bool_param;
    bool_param.name = name;
    bool_param.value = (item->checkState(1) == Qt::Checked);
    conf.bools.push_back(bool_param);
  }
  else if (item->text(1).contains('.')) {
    dynamic_reconfigure::DoubleParameter double_param;
    double_param.name = name;
    double_param.value = value.empty() ? 0 : std::stod(value);
    conf.doubles.push_back(double_param);
  }
  else {
    dynamic_reconfigure::IntParameter int_param;
    int_param.name = name;
    int_param.value = value.empty() ? 0 : std::stoi(value);
    conf.ints.push_back(int_param);
  }

  client_->update(conf);
}

} // end namespace rviz_reconfigure_plugin
