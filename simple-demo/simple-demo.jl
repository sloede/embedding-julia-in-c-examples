function double_me_julia_style(result, data)
  result .= 2*data

  return nothing 
end

function double_me_c_style(result_, data_)
  # Convert C types to Julia types
  result = unsafe_wrap(Array, result_, 10)
  data = unsafe_wrap(Array, data_, 10)

  result .= 2*data

  return nothing
end
