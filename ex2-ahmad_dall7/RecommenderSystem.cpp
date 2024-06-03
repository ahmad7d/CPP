#include "RecommenderSystem.h"
#include <math.h>

bool compare_function_map (const sp_movie &a, const sp_movie &b)
/**
 * a function that  compare between two movie (which one is older)
 * @param a first movie
 * @param b second movie
 * @return true if the first smaller than the sec else : false
 */
{

  return *a < *b;
}

static std::vector<double>
get_movie_features (const std::map<sp_movie, std::vector<double>,
    sp_movie_comapre_func> &movies, const sp_movie &movie)
    /**
     * getter method to get movie feature vector which allocated in movies map
     *  via the sp_movie as a key .
     * @param movies the rank map
     * @param movie the sp_movie
     * @return movie feature vector
     */
{

  return movies.find (movie)->second;
}

static double get_ranks_avg (const rank_map &ranks)
/**
 * a function that return the average of ranked movies by the user
 * @param ranks map that contain sp_movie as key and the rank as value
 * @return the average of the ranks
 */
{
  double sum = 0;
  for (const auto &it: ranks)
    {
      sum += it.second;
    }

  return ranks.empty () ? 0 : sum / ranks.size ();

}

static rank_map normalize_ranks (const rank_map &ranks)
/**
 * a function to decrease movie rank by the rank average
 * @param ranks ranked movies map
 * @return the rank after the decreasing process
 */
{
  rank_map norm_ranks (ranks);
  double ranks_avg = get_ranks_avg (norm_ranks);
  for (auto &it: norm_ranks)
    {
      it.second -= ranks_avg;
    }

  return norm_ranks;
}

static void multiply_vector (std::vector<double> &vec, double val)
/**
 * a function to multiply vector elements by given value
 * @param vec the vector
 * @param val
 */
{
  for (auto &num: vec)
    {
      num *= val;
    }
}

static void
add_to_vector (std::vector<double> &vec1, std::vector<double> &vec2)
/**
 * a function to sum two given vectors
 * @param vec1
 * @param vec2
 */
{
  if (vec1.size () != vec2.size ()) // checking if they have the same size
    {
      return;
    }

  for (size_t i = 0; i < vec1.size (); ++i)
    {
      vec1[i] += vec2[i];
    }
}

static double vector_norm (const std::vector<double> &vec)
/**
 * function that calculate vector norm
 * @param vec
 * @return sqrt of the vector inner product
 */
{
  double sum_power2 = 0;
  for (const auto &num: vec)
    {
      sum_power2 += pow (num, 2);
    }

  return sqrt (sum_power2);
}

static double
scalar_multiply_vec (const std::vector<double> &vec1,
                     const std::vector<double> &vec2)
/**
* a function to calculate scalar_multiply of two vectors
* @param vec1
* @param vec2
* @return sum of multiply each element in both vectors
*/
{
  double result = 0;
  if (vec1.size () == vec2.size ())
    {
      for (size_t i = 0; i < vec1.size (); ++i)
        {
          result += vec1[i] * vec2[i];
        }
    }
  return result;
}

static double
get_vec_similarity (const std::vector<double> &vec1,
                    const std::vector<double> &vec2)
/**
* the similarity process
* @param vec1
* @param vec2
* @return the similarity of the two given vectors
*/
{
  double norms_multiply = (vector_norm (vec1) * vector_norm (vec2));
  return norms_multiply == 0 ? 0 : scalar_multiply_vec (vec1, vec2)
                                   / norms_multiply; // return 0 if norm ==0
                                   // to avoid zero divisor
}

static std::vector<std::pair<sp_movie, double>>
get_most_similar_k_movies (const sp_movie &movie,
                           const rank_map &ranks,
                           const std::map<sp_movie,
                               std::vector<double>,
                               sp_movie_comapre_func> &movies,
                           int k)
/**
 * a function that create a vector who contain pairs of movies details
 * and the similarity .
 * sorting the vector after creating it to get the must similar k movies .
 * @param movie
 * @param ranks
 * @param movies
 * @param k
 * @return vector who contain pairs of movies details and the similarity
 */
{
  std::vector<std::pair<sp_movie, double>> ranked_similarities;
  std::vector<double> movie_features = get_movie_features (movies, movie);
  for (const auto &it: ranks)
    {
      ranked_similarities.emplace_back (it.first, get_vec_similarity
      (movie_features,get_movie_features (movies, it.first)));
    }

  std::sort (ranked_similarities.begin (), ranked_similarities.end (),
             [&] (const auto &a, const auto &b)
             { return b.second < a.second; });
  auto it = std::next (ranked_similarities.begin (), k);
  return std::vector<std::pair<sp_movie, double>>
      (ranked_similarities.begin (), it);
}

static std::vector<double> create_rc_vec (const rank_map &ranks,
                                          const std::map<sp_movie,
                                              std::vector<double>,
                                              sp_movie_comapre_func> &movies)
/**
 * creating the recommended vector to compare it with the other movies
 * @param ranks
 * @param movies
 * @return the normed recommendation_vector
 */
{
  rank_map norm_ranks = normalize_ranks (ranks);
  std::vector<double> recommendation_vector;

  for (auto &it: norm_ranks)
    {
      std::vector<double> movie_features (get_movie_features (movies,
                                                              it.first));
      multiply_vector (movie_features, it.second);
      if (recommendation_vector.empty ()) // if it's empty then make it
        {
          recommendation_vector = movie_features;
        }
      else // add it to the vector after the normalize process
        {
          add_to_vector (recommendation_vector, movie_features);
        }
    }
  return recommendation_vector;
}

static std::vector<sp_movie> get_unranked_movies (const rank_map &ranks,
                                                  const std::map<sp_movie,
                                                 std::vector<double>,
                                               sp_movie_comapre_func> &movies)
/**
 * a function to get all unranked movies and emplace them in a vector
 * if the movie doesnt exist in movies map , then its unranked
 * because we added only movies with rank in UsersLoader .
 * @param ranks
 * @param movies
 * @return unranked movies vector
 */
{
  std::vector<sp_movie> new_movies;
  for (const auto &it: movies)
    {
      sp_movie movie = it.first;
      if (ranks.find (movie) == ranks.end ())
        {
          new_movies.emplace_back (movie);
        }
    }

  return new_movies;
}

RecommenderSystem::RecommenderSystem () : rec_system (compare_function_map)
/**
 * constructor for the RecommenderSystem class , with adding the compare
 * function in it.
 */
{
}

sp_movie
RecommenderSystem::add_movie (const std::string &name, int year,
                              const std::vector<double> &features)
/**
 * method for adding movie details and features to movies map
 *  then printing the whole movies details in the map
 * @param name
 * @param year
 * @param features
 * @return the sp_move after making it shared
 */
{
  sp_movie movie = std::make_shared<Movie> (name, year);
  rec_system.emplace (movie, features);

  for (auto &it: rec_system)
    {
      std::cout << *(it.first) << std::endl;
    }

  return movie;
}

sp_movie RecommenderSystem::get_movie (const std::string &name, int year) const
/**
 * getter method that return movie details
 * @param name
 * @param year
 * @return movie details
 */
{
  auto it = rec_system.find (std::make_shared<Movie> (name, year));
  return it == rec_system.end () ? nullptr : it->first; // if it's no exist
  // return null , else return the movie details .
}
sp_movie RecommenderSystem::recommend_by_content (const RSUser &user)
/**
 * getting the recommended movie by content calculation
 * @param user
 * @return the recommended movie
 */
{
  sp_movie recommended_movie = nullptr;
  double max_similarity = -1;
  std::vector<double> recommendation_vector = create_rc_vec
      (user.get_ranks (), rec_system); // normalize the vector

  // getting all unranked movies
  auto new_movies = get_unranked_movies (user.get_ranks (),
                                         rec_system);
  for (auto &movie: new_movies)
    {
      double similarity = get_vec_similarity
          (recommendation_vector,
           get_movie_features (rec_system, movie));
      // if it is greater than the last movie make it the required movie
      // and place the max_similarly to continue compare process
      if (similarity > max_similarity)
        {
          recommended_movie = movie;
          max_similarity = similarity;
        }
    }
  return recommended_movie;
}

double
RecommenderSystem::predict_movie_score
    (const RSUser &user, const sp_movie &movie, int k)
/**
 * a method to get must similar movies then filtering them
 * @param user
 * @param movie
 * @param k
 * @return the ranking prediction
 */
{
  auto ranks = user.get_ranks ();
  auto movies = get_most_similar_k_movies (movie,
                                           ranks, rec_system, k);
  double s_sim = 0, s_rank_sim = 0;
  for (auto &it: movies) // the calculation
    {
      s_rank_sim += it.second * ranks.find (it.first)->second;
      s_sim += it.second;
    }
  return s_sim == 0 ? 0 : s_rank_sim / s_sim; // avoiding zero divisor
}


sp_movie RecommenderSystem::recommend_by_cf (const RSUser &user, int k)
/**
 * a method that return a filtered recommended movie
 * @param user
 * @param k
 * @return recommended movie
 */
{
  sp_movie result = nullptr;
  double max_pred = -1;
  std::vector<sp_movie> unranked_movies =
      get_unranked_movies (user.get_ranks(), rec_system);
  // getting the whole unranked movies
  for (auto &movie: unranked_movies)
    {
      double prediction = predict_movie_score (user, movie, k);
      // checking if the predication greater than the last one
      // then make it the required movie and place the max_prediction
      // to continue compare process
      if (prediction > max_pred)
        {
          result = movie;
          max_pred = prediction;
        }
    }
  return result;
}

std::ostream &operator<< (std::ostream &out, const RecommenderSystem &rs)
/**
 * << operator to transfer movie details for ostream
 * @param out
 * @param rs
 * @return
 */
{
  for (auto it: rs.rec_system)
    {
      out << *(it.first);
    }
  return out;
}
