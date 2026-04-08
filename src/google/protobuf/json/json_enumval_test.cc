// Protocol Buffers - Google's data interchange format
// Copyright 2026 Google LLC.  All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "absl/status/status.h"
#include "google/protobuf/json/json.h"
#include "google/protobuf/json/json_enumval.pb.h"

#ifndef EXPECT_OK
#define EXPECT_OK(x) EXPECT_TRUE(x.ok())
#endif  // EXPECT_OK

namespace google {
namespace protobuf {
namespace {

using json_enumval::Armor;
using json_enumval::Knight;

// Gorget does not have a custom json enumval string set, so it defaults to
// the original enumval: ARMOR_GORGET.
TEST(JsonEnumvalTest, GorgetDefaultSerialization) {
  Knight msg;
  msg.set_armor(Armor::ARMOR_GORGET);
  EXPECT_EQ(msg.armor(), Armor::ARMOR_GORGET);

  std::string json_res{};
  absl::Status status = json::MessageToJsonString(msg, &json_res);
  EXPECT_OK(status);
  EXPECT_EQ(json_res, R"json({"armor":"ARMOR_GORGET"})json");
}

// The great helm does have a custom json enumval, so let's confirm.
TEST(JsonEnumvalueOptionsTest, GreatHelmSerialization) {
  Knight msg;
  msg.set_armor(Armor::ARMOR_GREAT_HELM);
  EXPECT_EQ(msg.armor(), Armor::ARMOR_GREAT_HELM);

  std::string json_res{};
  absl::Status status = json::MessageToJsonString(msg, &json_res);
  EXPECT_OK(status);
  EXPECT_EQ(json_res, R"json({"armor":"gr8 helm"})json");
}

// Int overrides always win.
TEST(JsonEnumvalueOptionsTest, GreatHelmIntOverride) {
  Knight msg;
  msg.set_armor(Armor::ARMOR_GREAT_HELM);
  std::string json_res;
  json::PrintOptions print_options;
  print_options.always_print_enums_as_ints = true;
  absl::Status status =
      json::MessageToJsonString(msg, &json_res, print_options);
  EXPECT_OK(status);
  EXPECT_EQ(json_res, R"json({"armor":1})json");
}
}  // namespace
}  // namespace protobuf
}  // namespace google
