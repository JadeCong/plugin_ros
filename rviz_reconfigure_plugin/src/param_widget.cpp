#include "rviz_reconfigure_plugin/param_widget.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QMessageBox>

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

void ParamWidget::updateParams()
{
  std::string node_name = node_name_edit_->text().toStdString();
  
  // 使用 ros::Duration 来设置超时
  ros::Duration timeout(5.0);  // 5秒超时
  ros::Time start_time = ros::Time::now();

  while (ros::Time::now() - start_time < timeout)
  {
    dynamic_reconfigure::ReconfigureRequest req;
    dynamic_reconfigure::ReconfigureResponse res;

    if (ros::service::call(node_name + "/set_parameters", req, res))
    {
      updateParamTree(res.config);
      return;
    }
    ros::Duration(0.1).sleep();  // 等待100ms后重试
  }

  QMessageBox::warning(this, "Error", QString("Failed to connect to node %1 or get configuration").arg(QString::fromStdString(node_name)));
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
  if (column != 1) return;

  std::string node_name = node_name_edit_->text().toStdString();
  std::string param_name = item->text(0).toStdString();
  std::string value = item->text(1).toStdString();

  dynamic_reconfigure::ReconfigureRequest req;
  dynamic_reconfigure::ReconfigureResponse res;

  if (item->checkState(1) != Qt::PartiallyChecked)
  {
    dynamic_reconfigure::BoolParameter bool_param;
    bool_param.name = param_name;
    bool_param.value = (item->checkState(1) == Qt::Checked);
    req.config.bools.push_back(bool_param);
  }
  else if (item->text(1).contains('.'))
  {
    dynamic_reconfigure::DoubleParameter double_param;
    double_param.name = param_name;
    double_param.value = value.empty() ? 0 : std::stod(value);
    req.config.doubles.push_back(double_param);
  }
  else
  {
    dynamic_reconfigure::IntParameter int_param;
    int_param.name = param_name;
    int_param.value = value.empty() ? 0 : std::stoi(value);
    req.config.ints.push_back(int_param);
  }

  if (!ros::service::call(node_name + "/set_parameters", req, res))
  {
    QMessageBox::warning(this, "Error", "Failed to update parameter.");
  }
}

} // end namespace rviz_reconfigure_plugin
