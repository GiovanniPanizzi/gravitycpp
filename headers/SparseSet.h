#include <vector>
#include <type_traits>

template <typename T>
class SparseSet
{
    static_assert(std::is_unsigned<T>::value, "SparseSet can only contain unsigned integers");

private:
    std::vector<T> dense;   // Dense set of elements
    std::vector<T> sparse;  // Map of elements to dense set indices

    size_t size_ = 0;       // Current size (number of elements)
    size_t capacity_ = 0;   // Current capacity (maximum value + 1)

public:
    using iterator = typename std::vector<T>::const_iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    // Iterators
    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;

    // Size and capacity
    size_t size() const;
    size_t capacity() const;

    bool empty() const;

    // Clear the set
    void clear();

    // Reserve capacity for the set
    void reserve(size_t u);

    // Check if the set contains a value
    bool has(const T &val) const;

    // Insert a value into the set
    void insert(const T &val);

    // Erase a value from the set
    void erase(const T &val);
};