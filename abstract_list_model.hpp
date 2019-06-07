#pragma once

#include <QModelIndex>
#include <QVariant>
#include <QHash>
#include <QByteArray>
#include <QDebug>

#include <vector>
#include <utility>
#include <string>
#include <functional>


template<typename container_t>
class abstract_list_model_t
{
public:
  using value_type = typename container_t::value_type;
  using role_fn = std::function<QVariant(value_type const &)>;

public:
  template <typename QListModel>
  void set(const container_t& new_model, QListModel& list)
  {
    for(size_t i = 0; i < new_model.size() && i < model.size(); ++i) 
    {
      if(new_model[i] == model[i]) continue;

      model[i] = new_model[i];    

      QModelIndex topLeft = list.createIndex(i, 0);
      QModelIndex bottomRight = list.createIndex(i, 0);
      emit list.dataChanged(topLeft, bottomRight);
    }


    if(model.size() < new_model.size()) 
    {
      list.beginInsertRows(QModelIndex(), model.size(), new_model.size() - 1);
      model.insert(model.end(), new_model.begin() + model.size(), new_model.end());
      list.endInsertRows();
    }

    if(model.size() > new_model.size()) 
    {
      list.beginRemoveRows(QModelIndex(), new_model.size(), model.size() - 1);
      model.erase(model.begin() + new_model.size(), model.end());
      list.endRemoveRows();
    }
  }

  int rowCount(const QModelIndex& parent = QModelIndex()) const { Q_UNUSED(parent); return model.size(); }
  QVariant data(const QModelIndex& model_index, int role) const
  {
    if(!model_index.isValid()) return QVariant();
    const size_t index = static_cast<size_t>(model_index.row());
    const size_t urole = static_cast<size_t>(role - FIRST_ROLE_INDEX);

    try
    {
      return roles.at(urole).second(model.at(index));
    } catch (...) {}

    return QVariant();
  }
  
  QHash<int, QByteArray> roleNames() const
  {
    QHash<int, QByteArray> result;
    int index = FIRST_ROLE_INDEX;
    for (auto const &i : roles) result[index++] = i.first.c_str();

    return result;
  }

public:
  void add_role(std::string&& role_name, role_fn&& fn) { roles.emplace_back(std::move(role_name), std::move(fn)); }
  const value_type& get(size_t i) const { return model.at(i); }
  void set(container_t&& m) { model = std::move(m); }

private:
  enum { FIRST_ROLE_INDEX = Qt::UserRole + 1 };
  std::vector<std::pair<std::string, role_fn>> roles;
  container_t model;
};

