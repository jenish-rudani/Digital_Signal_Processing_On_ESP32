#include "../inc/filter.h"

ExponentialDiscreteFilter::ExponentialDiscreteFilter(float *val,
                                                     int length_of_filter,
                                                     float decay_alpha,
                                                     float rise_alpha) {
  len_of_filter = length_of_filter;
  exp_alpha_decay = decay_alpha;
  exp_alpha_rise = rise_alpha;

  filter_output_buffer = new float[length_of_filter];
  for (int i = 0; i < len_of_filter; i++) {
    filter_output_buffer[i] = val[i];
  }
}

ExponentialDiscreteFilter::ExponentialDiscreteFilter(int length_of_filter,
                                                     float decay_alpha,
                                                     float rise_alpha) {
  len_of_filter = length_of_filter;
  exp_alpha_decay = decay_alpha;
  exp_alpha_rise = rise_alpha;

  filter_output_buffer = new float[length_of_filter];
  for (int i = 0; i < len_of_filter; i++) {
    filter_output_buffer[i] = 0.0;
  }
}

ExponentialDiscreteFilter::~ExponentialDiscreteFilter() {
  delete[] filter_output_buffer;
}

float *ExponentialDiscreteFilter::get_filter_output_buffer() {
  return filter_output_buffer;
}

void ExponentialDiscreteFilter::update_smoothed_value_len_one(float *data) {
  for (int i = 0; i < len_of_filter; i++) {
    if (data[i] > filter_output_buffer[i]) {
      filter_output_buffer[i] =
          data[i] * exp_alpha_rise +
          filter_output_buffer[i] * (1.0 - exp_alpha_rise);
    } else {
      filter_output_buffer[i] =
          data[i] * exp_alpha_decay +
          filter_output_buffer[i] * (1.0 - exp_alpha_decay);
    }
  }
}

void ExponentialDiscreteFilter::update_smoothed_value(float *data) {
  for (int i = 0; i < len_of_filter; i++) {
    if (data[i] > filter_output_buffer[i]) {
      filter_output_buffer[i] =
          data[i] * exp_alpha_rise +
          filter_output_buffer[i] * (1.0 - exp_alpha_rise);
    } else {
      filter_output_buffer[i] =
          data[i] * exp_alpha_decay +
          filter_output_buffer[i] * (1.0 - exp_alpha_decay);
    }
    data[i] = filter_output_buffer[i];
  }
}

void ExponentialDiscreteFilter::update_smoothed_value(uint8_t *data) {
  for (int i = 0; i < len_of_filter; i++) {
    if (data[i] > filter_output_buffer[i]) {
      filter_output_buffer[i] =
          data[i] * exp_alpha_rise +
          filter_output_buffer[i] * (1.0 - exp_alpha_rise);
    } else {
      filter_output_buffer[i] =
          data[i] * exp_alpha_decay +
          filter_output_buffer[i] * (1.0 - exp_alpha_decay);
    }
    data[i] = filter_output_buffer[i];
  }
}