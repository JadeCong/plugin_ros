#ifndef RECONFIGURE_PANEL_H
#define RECONFIGURE_PANEL_H

#include <ros/ros.h>
#include <rviz/panel.h>
#include "param_widget.h"

namespace rviz_reconfigure_plugin {

class ReconfigurePanel : public rviz::Panel {
Q_OBJECT
public:
  ReconfigurePanel(QWidget* parent = 0);

  virtual void load(const rviz::Config& config);
  virtual void save(rviz::Config config) const;

protected:
  ParamWidget* param_widget_;
};

} // end namespace rviz_reconfigure_plugin

#endif // RECONFIGURE_PANEL_H
