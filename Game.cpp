#include "Game.hpp"

#include <chrono>
#include <random>
#include <QDebug>

namespace
{

static const int BALOON_MAX_SPEED = 30;
static const int BALOON_START_SPEED = 3;

inline void correct_x_position(baloon& b, size_t max_x)
{
  const int overflow = b.x + b.radius*2 - max_x;
  if(overflow > 0) b.x -= overflow;
}

inline int get_random(size_t max)
{
  return std::rand() % max;
}

inline bool is_ran_away(const baloon& el)
{
  return el.y < (-el.radius * 2);
}

inline bool game_finished(const baloon& el)
{
  return el.speed_px >= BALOON_MAX_SPEED;
}

}//namespace

/////////////////////////////////////baloon/////////////////////////////////////
baloon::baloon(size_t id, int x, int y)
  : id(id), x(x), y(y), radius(25), speed_px(BALOON_START_SPEED)
{
}

void baloon::increase_speed() 
{ 
  speed_px += speed_px/2; 
}

void baloon::update_y_coord()
{
  y = y - speed_px;
}

/////////////////////////////////////Game/////////////////////////////////////
Game::Game(unsigned screen_width, unsigned screen_height, size_t baloons_count, const std::chrono::milliseconds& update_interval)
  : m_ran_away_count(0) 
  , m_clicked_count(0)
  , m_screen_width(screen_width)
  , m_screen_height(screen_height)
  , timer(this)
{
  std::srand(std::time(nullptr));
  
  el_model.add_role("x_pos", [](const auto& el) { return el.x; });
  el_model.add_role("y_pos", [](const auto& el) { return el.y; });
  el_model.add_role("el_radius", [](const auto& el) { return el.radius; });
  
  for(size_t i = 0; i < baloons_count; ++i)
  {
    baloons.push_back({i, get_random(m_screen_width), m_screen_height});
    correct_x_position(baloons.back(), m_screen_width);
  }
  el_model.set(baloons, *this);
  
  connect(&timer, &QTimer::timeout, [&]()
  {
    for(size_t i = 0; i < baloons.size(); ++i) 
    {
      auto& el = baloons[i];
      el.update_y_coord();
      if(!is_ran_away(el)) continue;
      
      ran_away(i);
      el.increase_speed();
      if(!game_finished(el)) continue;
      emit finished();
      timer.stop();
    }
    el_model.set(baloons, *this);
  });
  timer.start(update_interval.count());
}

Game::~Game() = default;

int Game::screen_width() const
{
  return m_screen_width;
}

int Game::screen_height() const
{
  return m_screen_height;
}

int Game::ran_away_count() const
{
  return m_ran_away_count;
}

int Game::clicked_count() const
{
  return m_clicked_count;
}

void Game::baloon_clicked(int index)
{
  ++m_clicked_count;
  emit clicked_countChanged();
  
  auto& el = baloons[index];
  el.increase_speed();
  reset_position(el);
  el_model.set(baloons, *this);
}

void Game::ran_away(int index)
{
  ++m_ran_away_count;
  emit ran_away_countChanged();
  
  reset_position(baloons[index]);
  el_model.set(baloons, *this);
}

void Game::reset_position(baloon& el)
{
  el.y = m_screen_height;
  el.x = get_random(m_screen_width);
  correct_x_position(el, m_screen_width);
}
