// DynArray file beginning
#include <iterator>
#include <algorithm>
#include <stdexcept>
#include <limits>

// DynArray definition
template <class T>
class DynArray
{
public:
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using iterator = T*;
  using const_iterator = const T*;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

public:
  // Constructors
  explicit DynArray(size_type count)
    : _arr(new T[count]())
    , _size(count)
  {
  }

  DynArray(size_type count, const T& value)
    : _arr(new T[count])
    , _size(count)
  {
    std::fill_n(_arr, _size, value);
  }

  DynArray(const DynArray& other)
    : _arr(new T[other.size()]())
    , _size(other.size())
  {
    std::copy_n(other._arr, _size, _arr);
  }

  DynArray(std::initializer_list<T> init)
    : _arr(new T[init.size()]())
    , _size(init.size())
  {
    std::copy_n(init.begin(), _size, _arr);
  }

public:
  ~DynArray()
  {
    delete[] _arr;
  }

public:
  reference   at(size_type pos)
  {
    if (pos >= _size)
      throw std::out_of_range("pos is out of range");
    return _arr[pos];
  }

  const_reference at(size_type pos) const
  {
    if (pos >= _size)
      throw std::out_of_range("pos is out of range");
    return _arr[pos];
  }

  reference   operator[](size_type pos)
  {
    return _arr[pos];
  }

  const_reference operator[](size_type pos) const
  {
    return _arr[pos];
  }

  reference   front()
  {
    return _arr[0];
  }

  const_reference front() const
  {
    return _arr[0];
  }

  reference   back()
  {
    return _arr[_size - 1];
  }

  const_reference back() const
  {
    return _arr[_size - 1];
  }

  T*   data()
  {
    return _arr;
  }

  const T* data() const
  {
    return _arr;
  }

public:
  iterator begin()
  {
    return _arr;
  }

  const_iterator begin() const
  {
    return _arr;
  }

  const_iterator cbegin() const
  {
    return _arr;
  }

  iterator end()
  {
    return _arr + _size;
  }

  const_iterator end() const
  {
    return _arr + _size;
  }

  const_iterator cend() const
  {
    return _arr + _size;
  }

public:
  reverse_iterator rbegin()
  {
    return reverse_iterator(_arr + _size);
  }

  const_reverse_iterator rbegin() const
  {
    return const_reverse_iterator(_arr + _size);
  }

  const_reverse_iterator crbegin() const
  {
    return const_reverse_iterator(_arr + _size);
  }

  reverse_iterator rend()
  {
    return reverse_iterator(_arr);
  }

  const_reverse_iterator rend() const
  {
    return const_reverse_iterator(_arr);
  }

  const_reverse_iterator crend() const
  {
    return const_reverse_iterator(_arr);
  }

public:
  bool empty() const
  {
    return _size == 0;
  }

  size_type   size() const
  {
    return _size;
  }

  size_type   max_size() const
  {
    return std::numeric_limits<size_type>::max();
  }

public:
  void    fill(const T& value)
  {
    std::fill_n(_arr, _size, value);
  }

private:
  T*          _arr;
  size_type   _size;
};