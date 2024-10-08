#ifndef PARAM_WIDGET_H
#define PARAM_WIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <ros/ros.h>
#include <dynamic_reconfigure/client.h>

namespace rviz_reconfigure_plugin {

class ParamWidget : public QWidget {
Q_OBJECT
public:
  ParamWidget(QWidget* parent = 0);

private Q_SLOTS:
  void updateParams();
  void onItemChanged(QTreeWidgetItem* item, int column);

private:
  void updateParamTree(const dynamic_reconfigure::Config& config);

  QLineEdit* node_name_edit_;
  QPushButton* update_button_;
  QTreeWidget* param_tree_;
  std::map<std::string, QTreeWidgetItem*> param_items_;
};

} // end namespace rviz_reconfigure_plugin

#endif // PARAM_WIDGET_H
