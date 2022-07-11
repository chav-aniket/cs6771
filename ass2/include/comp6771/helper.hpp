#ifndef Z5265106_HELPER_HPP
#define Z5265106_HELPER_HPP

#include <memory>

inline auto itos(int i) -> std::size_t {
	return static_cast<std::size_t>(i);
}

// NOLINTNEXTLINE(modernize-avoid-c-arrays)
inline auto prep_mag(int i) -> std::unique_ptr<double[]> {
	// NOLINTNEXTLINE(modernize-avoid-c-arrays)
	return std::make_unique<double[]>(itos(i));
}

#endif // Z5265106_HELPER_HPP