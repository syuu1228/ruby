add_method_rule do
  list_mode = :blacklist

  list = ["BlackClass.blackFunction",
          "BlackClass2.blackFunction2",
          "BlackClass3.blackFunction3",
          "BlackClass4.blackFunction4",
          "BlackClass5.blackFunction5",
          "BlackClass6.blackFunction6"]
end

add_method_rule do
  list_mode = :whitelist

  list = ["WhiteClass.whiteFunction",
          "WhiteClass2.whiteFunction2",
          "WhiteClass3.whiteFunction3"]
end

add_require_rule do
  list_mode = :blacklist

  list = ["twitter",
          "json"]
end

add_require_rule do
  list_mode = :whitelist

  list = ["oauth",
          "cgi",
          "open-uri"]
end
