#pragma once

namespace duc {
	template <typename type>
	struct pair {
		type first;
		type second;
	};

	template <typename type1, typename type2>
	struct pair_any {
		type1 first;
		type2 second;
	};

	template <typename type>
	struct triplet {
		type first;
		type second;
		type third;
	};

	template <typename type1, typename type2, typename type3>
	struct triplet_any {
		type1 first;
		type2 second;
		type3 third;
	};
}