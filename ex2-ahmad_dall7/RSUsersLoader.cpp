#include "RSUsersLoader.h"
#include <fstream>
#include <sstream>
#include <string>

typedef std::pair<std::string, int> movie_entry;
#define MIN_RANK 1
#define MAX_RANK 10
#define RANK_ERROR "Rank is not in range"

static movie_entry get_movie(const std::string& movie_details)
/**
 * a method for splitting movie_details
 * @param movie_details
 * @return
 */
{
  // finding the separator index
  size_t separator = movie_details.find(SEPARATOR);
  // cutting the line and getting the name of the movie
  std::string name = movie_details.substr(0, separator);
  // converting the year number from string to integer
  int year = std::stoi(movie_details.substr
	(separator + 1, name.length() - (separator + 1)));
  return movie_entry(name, year); // make it pair [name, year]
}

static std::vector<movie_entry> extract_movies(const std::string& movies_s)
/**
 * extract movie details from the first line (name-year)
 * then adding it to a vector as pairs
 * each pair contain movie name and publish year
 * @param movies_s
 * @return
 */
{
	std::vector<movie_entry> movies;
	std::istringstream s_stream(movies_s);
	std::string movie_details;
	while (!s_stream.eof()) // while not reaching the end
	{
		s_stream >> movie_details;
		movies.emplace_back(get_movie(movie_details)); // emplace after
        // splitting merge them in pairs
	}
	return movies;
}


static RSUser create_user
(const std::string& user_data, const std::vector<movie_entry>& movies,
	recommender_system_p& rs)
/**
 * after getting the first line in the input file , this method
 * get each user name and ranks then emplace them in rank_map
 * @param user_data
 * @param movies
 * @param rs
 * @return
 */
{
	rank_map rank_map(0, sp_movie_hash, sp_movie_equal);
	std::istringstream line_stream(user_data);
	std::string name, in;
	double rank;

	line_stream >> name;
	for (size_t i = 0; i < movies.size(); ++i)
	{
		line_stream >> in;
		if (in == NA) // only need ranked movies
		{
			continue;
		}
		rank = std::stod(in);
      if (rank < MIN_RANK || rank > MAX_RANK) // checking valid rank
      {
        throw std::runtime_error(RANK_ERROR);
      }
		sp_movie movie =
			rs->get_movie(movies[i].first, movies[i].second);

		rank_map.emplace(movie, rank);

	}

	return RSUser(name, rank_map, rs);
}


std::vector<RSUser> RSUsersLoader::
create_users_from_file(const std::string& users_file_path,
	recommender_system_unique_p rs) noexcept(false)
/**
 * reading an input file that contain the whole movies in the first line ,
 * then usernames with them ranks for them movies
 * @param users_file_path
 * @param rs
 * @return
 */
{
	std::vector<RSUser> users;
	std::ifstream input(users_file_path, std::ifstream::in);
	std::string line;
	std::istringstream line_stream;
	std::vector<movie_entry> movies;
	recommender_system_p shared_rs = std::move(rs);
	if (!input.is_open())
	{
		throw std::runtime_error(FILE_ERR); // valid path file
	}
	std::getline(input, line); // get the first line which is the movies
	movies = extract_movies(line); // split the line [name-year]
	while (!input.eof()) // while not reaching the end of the file
	{
		std::vector<double> features;
		std::string user;
		std::getline(input, line); // get the first line
		if (line.length() == 0) // if the line is empty -> skip it
		{
			continue;
		}
		users.emplace_back(create_user(line, movies, shared_rs));
	}
	input.close(); // closing the file after reading it
	return users;
}
