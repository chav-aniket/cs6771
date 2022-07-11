cxx_test(
   TARGET euclidean_vector_test1
   FILENAME "euclidean_vector_test1.cpp"
   LINK euclidean_vector
)

cxx_test(
   TARGET euclidean_vector_constructor_test
   FILENAME "ev_constructor_test.cpp"
   LINK euclidean_vector
)

cxx_test(
   TARGET euclidean_vector_operator_test
   FILENAME "ev_operator_test.cpp"
   LINK euclidean_vector
)

cxx_test(
   TARGET euclidean_vector_friend_test
   FILENAME "ev_friend_test.cpp"
   LINK euclidean_vector
)

cxx_test(
   TARGET euclidean_vector_utility_test
   FILENAME "ev_utility_test.cpp"
   LINK euclidean_vector
)
