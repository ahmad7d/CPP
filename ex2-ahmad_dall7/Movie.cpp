
#include "Movie.h"
#define HASH_START 17
#define RES_MULT 31

/**
 * hash function used for a unordered_map (implemented for you)
 * @param movie shared pointer to movie
 * @return an integer for the hash map
 */


Movie::Movie (const std::string &name, int year)
/**
 * Constructor that get name as a string , and year as an integer .
 * @param name
 * @param year
 */
{
  m_name = name;
  m_year = year;
}

const std::string &Movie::get_name () const
/**
 * getter method to return the name of the movie .
 * @return movie name
 */
{
  return m_name;
}

int Movie::get_year () const
/**
 * getter method to return the year of the movie .
 * @return movie year
 */
{
  return m_year;
}

std::size_t sp_movie_hash (const sp_movie &movie)
{
  std::size_t res = HASH_START;
  res = res * RES_MULT + std::hash<std::string> () (movie->get_name ());
  res = res * RES_MULT + std::hash<int> () (movie->get_year ());
  return res;
}

/**
 * equal function used for an unordered_map (implemented for you)
 * @param m1
 * @param m2
 * @return true if the year and name are equal else false
 */
bool sp_movie_equal (const sp_movie &m1, const sp_movie &m2)
{
  return !(*m1 < *m2) && !(*m2 < *m1);
}

bool Movie::operator< (const Movie &movie) const
/**
 *  < operator that compare between two movies by the year .
 * @param movie
 * @return true if the left movie is older that the right movie else : false
 */
{
  return m_year < movie.m_year ||
         (m_year == movie.m_year && m_name < movie.m_name);
}

std::ostream &operator<< (std::ostream &out, const Movie &movie)
/**
 * << operator that transfer for to ostream the movie details
 * @param out ostream object
 * @param movie the movie object
 * @return
 */
{
  out << movie.m_name << " (" << movie.m_year << ")" << std::endl;

  return out;
}