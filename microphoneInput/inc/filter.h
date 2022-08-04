#pragma once

class ExponentialDiscreteFilter {
 private:
  float *filter_output_buffer;
  int len_of_filter;
  float exp_alpha_decay;
  float exp_alpha_rise;

 public:
  ExponentialDiscreteFilter(float *val, int val_len, float alpha_decay,
                            float alpha_rise);
  ExponentialDiscreteFilter(int val_len, float alpha_decay, float alpha_rise);
  ~ExponentialDiscreteFilter();
  void update_smoothed_value(uint8_t *data);
  void update_smoothed_value_len_one(float *data);
  void update_smoothed_value(float *data);
  float *get_filter_output_buffer();
};
