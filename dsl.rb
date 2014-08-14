#encoding:utf-8
class SecureDSL
  def initialize
    @require_rules = {
      "rules_count" => 0,
      "rules"       => [],
      "list_modes"  => []
    }

    @method_rules  = {
      "rules_count" => 0,
      "rules"       => [],
      "list_modes"  => []
    }

    @c_code = []
  end

  def load_file(fname)
    line       = 0
    begin_flag = false
    file_data  = File.read(fname).split("\n")

    file_data.each do |element|
      if element =~ /^#.*$/
        file_data[line] = ""
        line += 1
        next
      end

      if element =~ /add_(method|require)/
        file_data[line] = element
        begin_flag      = true
        line           += 1
        next
      end

      if begin_flag && element =~ /end/
        file_data[line] = "next [list_mode, list]; end"
        begin_flag      = false
        line           += 1
        next
      end

      line           += 1
    end
    
    eval(file_data.join("\n"))
  end

  def start_begin
    gen_c_code
  end

  def add_method_rule(&block)
    rule = block.call
    @method_rules["rules_count"] += 1
    #@method_rules["rules"] << rule
    @method_rules["rules"][@method_rules["rules_count"] - 1] = rule
  end

  def add_require_rule(&block)
    rule = block.call
    @require_rules["rules_count"] += 1
    @require_rules["rules"][@require_rules["rules_count"] - 1] = rule
  end

  def gen_c_code_run
    gen_c_code :method
    gen_c_code :require
  end

=begin
{{{
  def gen_c_code_(mode)
    if mode == :method
      long  = "method"
      short = "mi"
    elsif mode == :require
      long  = "require"
      short = "rq"
    end

    @c_code << "struct #{long}_information #{short}_array[] = {"

    list_mode    = nil    
    mi_ary_len  = 0
    rq_ary_len  = 0
    rules_count = 0
    eval("rules_count = @#{long}_rules[\"rules_count\"]")

    rules_count.times do |i|
      tmp_rules = []

      element_array = nil
      eval("element_array = @#{long}_rules[\"rules\"][i]")
      list_mode   = element_array[0]
      loop_conter = 0 

      element_array[1].each do |line|
        class_name  = line.split(".")[0]
        method_name = line.split(".")[1]

        if mode == :method
          base = "{\"#{class_name}\", \"#{method_name}\"}" 
        elsif mode == :require
          base = "{\"#{class_name}\"}"
        end

        base += "," unless loop_conter == element_array[1].size - 1

        loop_conter += 1
        tmp_rules << base
        eval("#{short}_ary_len += 1")
      end

      tmp_rules.each do |line|
        @c_code << line
      end
    end
    @c_code << "};"

    if list_mode == :blacklist
      @c_code << "int #{short}_blacklist = 1;"
      @c_code << "int #{short}_array_len = " + (short == "mi" ? mi_ary_len.to_s : rq_ary_len.to_s) + ";"
    elsif list_mode == :whitelist
      @c_code << "int #{short}_blacklist = 0;"
      @c_code << "int #{short}_array_len = " + (short == "mi" ? mi_ary_len.to_s : rq_ary_len.to_s) + ";"
    end
  end
  }}}
=end

  def gen_c_code(mode)
    if mode == :method
      long  = "method"
      short = "mi"
    elsif mode == :require
      long  = "require"
      short = "rq"
    end


    list_mode    = nil    
    mi_ary_lens = []
    rq_ary_lens = []
    rules_count = 0
    eval("rules_count = @#{long}_rules[\"rules_count\"]")

    rules_count.times do |i|
      @c_code << "struct #{long}_information #{short}_array#{i}[] = {"
      tmp_rules = []

      element_array = nil
      eval("element_array = @#{long}_rules[\"rules\"][i]")
      eval("@#{long}_rules[\"list_modes\"][i] = element_array[0]")
      loop_conter = 0 

      element_array[1].each do |line|
        class_name  = line.split(".")[0]
        method_name = line.split(".")[1]

        if mode == :method
          base = "{\"#{class_name}\", \"#{method_name}\"}" 
        elsif mode == :require
          base = "{\"#{class_name}\"}"
        end

        base += "," unless loop_conter == element_array[1].size - 1

        loop_conter += 1
        tmp_rules << base
      end

      tmp_rules.each do |line|
        @c_code << line
        eval("if #{short}_ary_lens[#{i}] == nil
                #{short}_ary_lens[#{i}] = 1
              else
                #{short}_ary_lens[#{i}] += 1
              end
              ")
      end

      @c_code << "};"
    end
    
    @c_code << "struct #{short}_ruleset #{short}_rulesets[] = {"
    rulesets_len = 0
    rules_count.times do |i|
      list_mode = nil
      eval("list_mode = @#{long}_rules[\"list_modes\"][i]")
      base = "{#{short}_array#{i}, #{(short == "mi" ? mi_ary_lens[i].to_s : rq_ary_lens[i].to_s)}, #{list_mode == :blacklist ? 1 : 0}}"
      base += "," unless i == rules_count - 1
      
      @c_code << base
      rulesets_len += 1
    end
    @c_code << "};"
    @c_code << "int #{short}_rulesets_len = #{rulesets_len};"
  end

  def output_c_code
    puts @c_code
  end
end

if ARGV.length == 0
  puts "This script require arguments at least 1"
  puts "Usage : ruby dsl.rb \"rule file name\""
  exit
end
ARGV.each{|fname|
  unless File.exist?(fname)
    puts "File not found - #{fname}"
    exit
  end
  sd = SecureDSL.new
  sd.load_file(fname)
  sd.gen_c_code_run
  sd.output_c_code
}
