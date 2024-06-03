#ifndef DICTIONARY_H
#define DICTIONARY_H
#include "HashMap.hpp"
#include <string>

class Dictionary : public HashMap<std::string, std::string>
{
 public:

  Dictionary ()
  {}

  Dictionary (const std::vector<std::string> &keys,
              const std::vector<std::string> &values)
      : HashMap (keys, values)
  {}

  /**
   * method to erase value of the given key
   * @param key
   * @return
   */
  bool erase (const std::string key) override // override to run over
  // the method in the base class
  {
    bool rc = HashMap<std::string, std::string>::erase (key);
    if (!rc)
    {
      throw InvalidKey ();
    }
    return rc;
  }

  void update (const bucket_t::iterator &it1, const bucket_t::iterator &it2)
  {
    bucket_t::iterator it = it1;
    while (it != it2)
    {
      if (!insert ((*it).first, (*it).second))
      {
        (*this)[(*it).first] = (*it).second;
      }
      it++;
    }
  }
};
#endif
