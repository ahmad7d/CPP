//
// Created on 2/20/2022.
//

// don't change those includes
#include "RSUser.h"
#include "RecommenderSystem.h"


// implement your cpp code here

double RSUser::get_rank_average (const rank_map &rankMap)
/**
 * calculate rank average
 * @param rankMap
 * @return the rank average
 */
{
  double s = 0;
  for (const auto &it: rankMap)
    {
      s += it.second;
    }
  return rankMap.empty() ? 0 : s / rankMap.size (); // avoid zero divisor
}

RSUser::RSUser (const std::string &name, const rank_map &map,
                recommender_system_p rs)
    : m_name (name), m_rank_map (map), m_rs (rs)
/**
 * constructor to build the object
 * @param name
 * @param map
 * @param rs
 */
{
}

const std::string &RSUser::get_name () const
/**
 * getter method to get user name
 * @return
 */
{
  return m_name;
}

void RSUser::add_movie_to_rs (const std::string &name, int year,
                              const std::vector<double> &features, double rate)
/**
 * a method for adding movie to the rank map
 * using the add_movie that make it shared pointer then emplace it in rank map
 * @param name
 * @param year
 * @param features
 * @param rate
 */
{
  sp_movie movie = m_rs->add_movie (name, year, features);
  m_rank_map.emplace (movie, rate);
}

const rank_map &RSUser::get_ranks () const
/**
 * getter method that return user rank map
 * @return
 */
{
  return m_rank_map;
}

sp_movie RSUser::get_recommendation_by_content () const
/**
 * a method that return the recommended movie by content
 * @return
 */
{
  return m_rs->recommend_by_content (*this);
}

sp_movie RSUser::get_recommendation_by_cf (int k) const
/**
 * a method that return the recommended movie by cf
 * @param k
 * @return
 */
{
  return m_rs->recommend_by_cf (*this, k);
}

double RSUser::get_prediction_score_for_movie
    (const std::string &name, int year, int k) const
/**
 * a method to get prediction score for movie
 * @param name
 * @param year
 * @param k
 * @return
 */
{
  sp_movie movie = m_rs->get_movie (name, year);
  // find the movie by its name and year in the recommended system
  return m_rs->predict_movie_score (*this, movie, k);
}

std::ostream &operator<< (std::ostream &out, const RSUser &user)
/**
 * << operator to transfer user name and movies details in rank map after
 * sorting it by < operator
 * @param out
 * @param user
 * @return
 */
{
  std::vector<std::pair<sp_movie, double>>
      rank_map (user.m_rank_map.begin (), user.m_rank_map.end ());
  // create pair vector (movie details as first , its rank as second)
  std::sort (rank_map.begin (), rank_map.end (),
             [&] (const auto &a, const auto &b)
             { return a.second < b.second; });
  out << "name: " << user.m_name << std::endl;
  for (const auto &it: rank_map)
    {
      out << *(it.first);
    }
  return out;
}
