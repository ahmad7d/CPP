#include "RecommenderSystemLoader.h"
#include <fstream>
#include <sstream>



static void add_movie
(const recommender_system_unique_p& rs, const std::string& movie_details,
	const std::vector<double>& features)
/**
 * a method for splitting movie_details which it's the rest of the line
 * we get in the input file , then adding it's details
 * for the recommended system
 * @param rs
 * @param movie_details
 * @param features
 */
{
  // finding the separator index
  size_t separator = movie_details.find(SEPARATOR);
  // cutting the line and getting the name of the movie
  std::string name = movie_details.substr(0, separator);
  // converting the year number from string to integer
  int year =
      atoi(movie_details.substr(separator + 1,
          name.length() - (separator + 1)).c_str());
  // adding all of the details for the recommendation system
  rs->add_movie(name, year, features);
}


recommender_system_unique_p RecommenderSystemLoader::create_rs_from_movies_file
(const std::string& movies_file_path) noexcept(false)
/**
 * reading the given input file and adding the information to the system
 * @param movies_file_path
 * @return the recommendation system after adding the information
 */
{
	recommender_system_unique_p rs = std::make_unique<RecommenderSystem>();
	std::ifstream input(movies_file_path, std::ifstream::in);
	std::string line;
	std::istringstream line_stream;

	if (!input.is_open())
	{
		throw std::runtime_error(FILE_ERR); // invalid file path
	}
	while (!input.eof()) // while we are not at END OF FILE
	{
		std::vector<double> features;
		std::string move_details;
		std::getline(input, line); // getting the first line
		if (line.length() == 0) // it might be empty file !
		{
			continue;
		}
		line_stream = std::istringstream(line);
		line_stream >> move_details;
		while (!line_stream.eof())
		{
			double feature;
			line_stream >> feature;
            if (feature < MIN_MOVIE_FEATURE || feature > MAX_MOVIE_FEATURE)
            {
              throw std::runtime_error(FEATURE_ERR); // invalid movie
              // feature (should between 1 - 10)
            }
			features.push_back(feature); // adding it to the features victor
		}
		add_movie(rs, move_details, features); // add all the
        // information to the system
	}
	input.close(); // closing the file after reading all of it
	return rs;
}
