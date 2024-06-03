
#ifndef RECOMMENDERSYSTEMLOADER_H
#define RECOMMENDERSYSTEMLOADER_H

#include "RecommenderSystem.h"

#define SEPARATOR '-'
#define NA "NA"
#define FILE_ERR "Error occurred while opening the file"
#define FEATURE_ERR "invalid feature , feature should be between 1-10"
#define MIN_MOVIE_FEATURE 1
#define MAX_MOVIE_FEATURE 10

class RecommenderSystemLoader {

private:

public:
	RecommenderSystemLoader() = delete;
	/**
	 * loads movies by the given format for movies with their feature's score
	 * @param movies_file_path a path to the file of the movies
	 * @return smart pointer to a RecommenderSystem which was created with
	 * those movies
	 */
	static recommender_system_unique_p create_rs_from_movies_file
	(const std::string& movies_file_path) noexcept (false);
};

#endif //RECOMMENDERSYSTEMLOADER_H
