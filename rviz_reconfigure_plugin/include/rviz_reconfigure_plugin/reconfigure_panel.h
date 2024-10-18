#ifndef DYNAMIC_RECONFIGURE_PANEL_H
#define DYNAMIC_RECONFIGURE_PANEL_H

#include <ros/ros.h>
#include <rviz/panel.h>
#include <dynamic_reconfigure/client.h>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTreeWidget>

namespace rviz_dynamic_reconfigure_plugin {

class DynamicReconfigurePanel : public rviz::Panel {
Q_OBJECT
public:
  DynamicReconfigurePanel(QWidget* parent = 0);

  virtual void load(const rviz::Config& config);
  virtual void save(rviz::Config config) const;

public Q_SLOTS:
  void updateNodeName();
  void updateParams();

protected:
  QLineEdit* node_name_editor_;
  QPushButton* update_button_;
  QTreeWidget* param_tree_;

  std::string node_name_;
  boost::shared_ptr<dynamic_reconfigure::Client<dynamic_reconfigure::Config>> client_;

  void displayParams(const dynamic_reconfigure::Config& config);
};

} // end namespace rviz_dynamic_reconfigure_plugin

#endif // DYNAMIC_RECONFIGURE_PANEL_H
