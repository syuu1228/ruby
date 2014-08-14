# id -> 0
add_method_rule do
  list_mode = :blacklist

  list  = ["BlackClass.blackFunction"]
end

add_require_rule do
  list_mode = :blacklist

  list = ["twitter"]
end


# id -> 1
add_method_rule do
  list_mode = :blacklist

  list  = ["BlackClass2.blackFunction2"]
end

add_require_rule do
  list_mode = :blacklist

  list = ["oauth"]
end
