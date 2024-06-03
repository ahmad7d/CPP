#ifndef HASH_MAP_H
#define HASH_MAP_H
#include <vector>
#include <stdexcept>

#define NXKEY_ERROR "key does not exist"
#define KEY_VALUE_SIZE_ERROR "vectors sizes are not equal"

#define BOTTOM_FACTOR 0.25
#define TOP_FACTOR 1-BOTTOM_FACTOR
#define INITIAL_SIZE 16
#define NOT_FOUND -1
#define ZERO 0

class InvalidKey : public std::invalid_argument
{
 public:
  InvalidKey () : std::invalid_argument (NXKEY_ERROR)
  {}
  InvalidKey (const std::string &err_msg) : std::invalid_argument (err_msg)
  {}
};


template<typename KeyT, typename ValueT>
/**
 *
 * @tparam KeyT
 * @tparam ValueT
 */
class HashMap
{
 public:
  typedef std::pair<KeyT, ValueT> element_t;
  typedef std::vector<element_t> bucket_t;

  /**
   * Implementing a const forward iterator by getting the hashmap and default
   * index for it .
   * const forward iterator only need one direction
   */
  class Iterator
  {
    typedef std::forward_iterator_tag iterator_category;
    typedef const element_t value_type;
    typedef const value_type &reference;
    typedef const value_type *pointer;
    typedef std::ptrdiff_t difference_type;

   public:
    /**
     * default constructor that get the hashmap and call
     * the iterator with index 1
     * @param map
     */
    explicit Iterator (HashMap *map)
        : Iterator (1, map)
    {}

    /**
     * implementing copy constructor
     * @param it
     */
    Iterator (const Iterator &it)
    {
      *this = it;
    }

    /**
     * constructor that get an element
     * @param i
     * @param map
     */
    Iterator (int i, HashMap *map)
        : m_index ((i > map->m_size || i < 1) ? -1 : i)
    {
      m_map = map;
    }

    /**
     * reference operator
     * @return reference to the i"th element of the hashmap
     */
    reference operator* () const
    {
      return m_map->get_ith_element (m_index);
    }

    /**
     * pointer operator
     * @return pointer to the i"th element of the hashmap
     */
    pointer operator-> () const
    {
      return &this->operator* ();
    }

    /**
     * ++ operator to get the next index
     * @return object after pointing to the next index
     */
    Iterator operator++ (int)
    {
      auto it = *this;
      ++*this;
      return it;
    }

    /**
     * ++ operator to get the next index
     * @return object after pointing to the next index
     */
    Iterator operator++ ()
    {
      m_index = m_index < 0 || m_index >= m_map->size () ? -1 :
                m_index + 1;
      return *this;
    }

    /**
     * comparison operator between two iterators
     * @param it
     * @return true if it the same iterator : else false
     */
    bool operator== (const Iterator &it) const
    {
      return m_index == it.m_index && m_map == it.m_map;
    }

    /**
     * comparison operator between two iterators
     * @param it
     * @return false if it the same iterator : else true
     */
    bool operator!= (const Iterator &it) const
    {
      return !(*this == it);
    }

    /**
     * assignment operator to assign iterator in another
     * @param it
     * @return the assigned iterator
     */
    Iterator &operator= (const Iterator &it)
    {
      m_map = it.m_map;
      m_index = it.m_index;
      return *this;
    }

   private:
    int m_index;
    HashMap *m_map;
  };

 private:

  int m_capacity, m_size; // private variables for the hashmap capacity, size
  bucket_t *m_buckets; // pointer two the buckets
  std::hash<KeyT> m_hasher; // the hash function

  /**
   * hash function to choose element's location
   * @param key
   * @return the location of the key in the hashmap
   */
  int hash (const KeyT &key) const
  {
    return m_hasher (key) & (m_capacity - 1);
  }

  /**
   * safe \ const method to get the index of giving key in the hashmap
   * @param key
   * @return
   */
  int find_element_index_safe (const KeyT &key) const
  {
    int i = find_element_index (key);
    if (i == NOT_FOUND)
    {
      throw InvalidKey ();
    }
    return i;
  }

  int find_element_index (const KeyT &key) const
  {

    int element_index = 0;

    for (auto &element: m_buckets[hash (key)])
    {
      if (element.first == key)
      {
        return element_index;
      }
      element_index++;
    }
    return NOT_FOUND;
  }

  /**
   * method two copy hashmap elements two another
   * @param src
   * @param src_capacity
   * @return size of it , not that much relevant !
   */
  int insert_elements (bucket_t *src, int src_capacity)
  {
    int count = 0;
    for (int i = 0 ; i < src_capacity ; ++i)
    {
      for (auto &it: src[i])
      {
        insert (it.first, it.second);
      }
      count += (int) src[i].size ();
    }
    return count;
  }

  /**
   * method to increase hashmap capacity
   */
  void grow ()
  {
    // copy of the old buckets and capacity
    bucket_t *old_buckets = m_buckets;
    int old_capacity = m_capacity;

    // change old capacity to the new one , size back to ZERO
    m_capacity *= 2, m_size = ZERO;
    m_buckets = init_buckets (m_capacity);

    // copy the old elements two the new bucket
    insert_elements (old_buckets, old_capacity);

    // free the allocated memory of the old bucket
    delete[] old_buckets;
  }

  /**
   * method to decrease hashmap capacity
   */
  void shrink ()
  {
    // copy of the old buckets and capacity
    bucket_t *old_buckets = m_buckets;
    int old_capacity = m_capacity;

    // getting the lowest possible capacity
    do
    {
      m_capacity /= 2;
    }
    while (get_load_factor () < BOTTOM_FACTOR && m_size > 0);

    m_size = ZERO;
    m_buckets = init_buckets (m_capacity);

    // copy the old elements two the new bucket
    insert_elements (old_buckets, old_capacity);

    // free the allocated memory of the old bucket
    delete[] old_buckets;
  }


  /**
   * get the ith pair in the hashmap
   * @param i
   * @return the ith pair
   */
  element_t &get_ith_element (int i) const
  {
    int bucket_i = 0;
    while (i > 0)
    {
      if (i <= (int) m_buckets[bucket_i].size ())
      {
        break;
      }
      i -= (int) m_buckets[bucket_i].size ();
      bucket_i++;
    }
    return m_buckets[bucket_i][i - 1];
  }

  /**
   * method to allocate new memory for the hashmap
   * @param size
   * @return the hashmap
   */
  bucket_t *init_buckets (int size)
  {
    return new (std::nothrow) bucket_t[size * sizeof (bucket_t)];
  }

 public:
  /**
   * default constructor that generate new hashmap with default size/capacity
   */
  HashMap ()
      : m_capacity (INITIAL_SIZE), m_size (ZERO)
  {
    m_buckets = init_buckets (m_capacity);
  }

  /**
   * constructor that get keys and values vectors to insert them in the hashmap
   * @param keys
   * @param values
   */
  HashMap (std::vector<KeyT> keys, std::vector<ValueT> values)
      : m_capacity (INITIAL_SIZE), m_size (ZERO)
  {
    if (keys.size () != values.size ())
    {
      throw std::runtime_error (KEY_VALUE_SIZE_ERROR);
    }

    m_buckets = init_buckets (m_capacity); // allocate memory for the hashmap

    for (size_t i = 0 ; i < keys.size() ; ++i)
    {
      if (contains_key (keys[i]))
      {
        // of the key already in the hashmap , change the value
        at (keys[i]) = values[i];
      }
      // else insert new key with it new value
      insert (keys[i], values[i]);
    }
  }

  /**
   * copy constructor that use the assigment operator
   * @param hash_map
   */
  HashMap (const HashMap<KeyT, ValueT> &hash_map)
  {
    *this = hash_map;
  }

  /**
   * destructor to free allocated memory for the hashmap
   */
  virtual ~HashMap ()
  {
    delete[] m_buckets;
    m_buckets = NULL;
    m_size = ZERO;
    m_capacity = ZERO;
  }

  /**
   * method two get the size of the hashmap
   * @return
   */
  int size () const
  {
    return m_size;
  }

  /**
   * method to get the capacity of the hashmap
   * @return
   */
  int capacity () const
  {
    return m_capacity;
  }

  /**
   * method that check of the hashmap empty or not
   * @return true if it empty : else false
   */
  bool empty () const
  {
    return m_size == ZERO;
  }

  /**
   * method to insert new key and value to the hashmao
   * @param key
   * @param value
   * @return true if the process success : else false
   */
  bool insert (KeyT key, ValueT value)
  {
    if (contains_key (key))
    {
      return false;
    }
    m_buckets[hash (key)].emplace_back (key, value);

    m_size++;

    if (get_load_factor () > TOP_FACTOR) // reached the limit
    {
      grow (); // increase the capacity
    }
    return true;
  }

  /**
   * method that check if the given key exist in the hashmap
   * @param key
   * @return true if it's exist : else false
   */
  bool contains_key (KeyT key) const
  {
    return find_element_index (key) >= ZERO;
  }

  /**
   * method that get the value of the given key
   * @param key
   * @return
   */
  ValueT &at (KeyT key)
  {
    int i = find_element_index_safe (key);
    return m_buckets[hash (key)][i].second;
  }

  /**
   * method that erase the value of the given key
   * @param key
   * @return true if the process success : else false
   */
  virtual bool erase (KeyT key)
  {
    auto i = find_element_index (key);
    bucket_t *bucket = &m_buckets[hash (key)];

    if (i < ZERO) // the key not found
    {
      return false;
    }

    bucket->erase (bucket->begin () + i); // deleting it using the erase method
    m_size--; // decrease the size of the hashmap

    if (get_load_factor () < BOTTOM_FACTOR) // reached the limit
    {
      shrink (); // decrease the capacity
    }

    return true;
  }

  /**
   * method that get the load factor of the hashmap
   * @return
   */
  double get_load_factor () const
  {
    return m_capacity == 0 ? 0 : (double) m_size / m_capacity;
  }

  /**
   * method that get the size of the given key bucket
   * @param key
   * @return
   */
  int bucket_size (KeyT key) const
  {
    find_element_index_safe (key);
    return (int) m_buckets[hash (key)].size ();
  }

  /**
   * method the get the index/location of the given key vector
   * @param key
   * @return
   */
  int bucket_index (KeyT key) const
  {
    find_element_index_safe (key);
    return hash (key);
  }

  /**
   * method that clear the vectors inside the hashmap
   */
  void clear ()
  {
    for (int i = 0 ; i < m_capacity ; ++i)
    {
      m_buckets[i].clear ();
    }
    m_size = ZERO;
  }

  /**
   * iterator that call the iterator constructor with default index
   * @return
   */
  Iterator begin ()
  {
    return m_size > ZERO ? Iterator (1, this) : end ();
  }

  /**
   * iterator that call the iterator constructor with the last + 1 index
   * @return
   */
  Iterator end ()
  {
    return Iterator (-1, this);
  }

  /**
   * const begin iterator
   * @return
   */
  Iterator cbegin ()
  {
    return begin ();
  }

  /**
   * const end iterator
   * @return
   */
  Iterator cend ()
  {
    return end ();
  }

  /**
   * assigment operator that copy old hashmap filedes (hashmap, capacity, size)
   * to a new one (deep copy)
   * @param hash_map
   * @return the new hashmap
   */
  HashMap<KeyT, ValueT> &operator= (const HashMap<KeyT, ValueT> &hash_map)
  {
    m_capacity = hash_map.m_capacity;
    m_size = 0;
    m_buckets = init_buckets (m_capacity);

    insert_elements (hash_map.m_buckets, m_capacity);

    return *this;
  }

  /**
   * [] operator to get the value of the given key in the hashmap
   * @param key
   * @return
   */
  ValueT &operator[] (KeyT key)
  {
    if (!contains_key (key))
    {
      insert (key, ValueT{});
    }
    return m_buckets[hash (key)][find_element_index (key)].second;
  }

  /**
   * const one
   * @param key
   * @return
   */
  const ValueT &operator[] (KeyT key) const
  {
    return m_buckets[hash (key)][find_element_index (key)].second;;
  }

  /**
   * compression operator between two objects
   * @param hash_map
   * @return true if they are equal : else false
   */
  bool operator== (const HashMap<KeyT, ValueT> &hash_map) const
  {
    if (m_capacity != hash_map.m_capacity || m_size != hash_map.m_size)
    {
      return false;
    }

    for (int bucket_i = 0 ; bucket_i<m_capacity ; ++bucket_i)
    {
      auto bucket_1 = m_buckets[bucket_i],
          bucket_2 = hash_map.m_buckets[bucket_i];

      if (bucket_1.size () != bucket_2.size ())
      {
        return false;
      }

      for (int i = 0 ; i<(int)bucket_1.size() ; ++i)
      {
        bool found = false;
        for (int j = 0 ; j<(int)bucket_1.size() && !found ; ++j)
        {
          found |= bucket_1[i].first == bucket_2[j].first &&
                   bucket_1[i].second == bucket_2[j].second;
        }
        if (!found)
        {
          return false;
        }
      }
    }
    return true;
  }

  /**
   * compression operator between two objects
   * @param hash_map
   * @return false if they are equal : else true
   */
  bool operator!= (const HashMap<KeyT, ValueT> &hash_map) const
  {
    return !(hash_map == *this);
  }
};

#endif
