#pragma once

#include "abstract_list_model.hpp"

#include <vector>
#include <memory>
#include <QAbstractListModel>
#include <QTimer>
#include <chrono>

struct baloon
{
  baloon(size_t id, int x, int y);
  void update_y_coord();
  void increase_speed();
  
  size_t id;
  int x;
  int y;
  int radius;
  int speed_px;
};

inline bool operator==(const baloon& lhs, const baloon& rhs)
{
  return lhs.id == rhs.id && lhs.x == rhs.x && lhs.y == rhs.y;
}

class Game : public QAbstractListModel
{
  Q_OBJECT

  Q_PROPERTY(unsigned screen_width READ screen_width CONSTANT)
  Q_PROPERTY(unsigned screen_height READ screen_height CONSTANT)
  Q_PROPERTY(int ran_away_count READ ran_away_count NOTIFY ran_away_countChanged)
  Q_PROPERTY(int clicked_count READ clicked_count NOTIFY clicked_countChanged)

public:
  Q_INVOKABLE void baloon_clicked(int index);

public:
  Game(unsigned screen_width, unsigned screen_height, size_t baloons_count, const std::chrono::milliseconds& update_interval);
  virtual ~Game();
  
  int rowCount(const QModelIndex &parent = QModelIndex()) const override { return el_model.rowCount(parent); }
  QVariant data(const QModelIndex &model_index, int role) const override { return el_model.data(model_index, role); }
  QHash<int, QByteArray> roleNames() const override { return el_model.roleNames(); }  
  
signals:
  void ran_away_countChanged();
  void clicked_countChanged();
  void finished();
  

private:
  int screen_width() const;
  int screen_height() const;
  int ran_away_count() const;
  int clicked_count()  const;
  void ran_away(int index);
  void reset_position(baloon& el);
  
private:
  int m_ran_away_count;
  int m_clicked_count;
  const unsigned m_screen_width;
  const unsigned m_screen_height;
  std::vector<baloon> baloons;
  abstract_list_model_t<std::vector<baloon>> el_model;
  QTimer timer;
  
  template<typename> friend class abstract_list_model_t;
};

