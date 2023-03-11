// Copyright 2021-2022 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// clang-format off

// Macro for conveniently defining Dactyl layouts.
#define LAYOUT_wrapper(...) LAYOUT_ortho_5x15(__VA_ARGS__)

#define LAYOUT_LR(                          \
    L00, L01, L02, L03, L04, L05,           \
    L10, L11, L12, L13, L14, L15,           \
    L20, L21, L22, L23, L24, L25,           \
    L30, L31, L32, L33, L34, L35,           \
    L40, L41, L42, L43, L44, L45,           \
                                            \
                M00, M01, M02,              \
                M10, M11, M12,              \
                M20, M21, M22,              \
                M30, M31, M32,              \
                M40, M41, M42,              \
                                            \
              R00, R01, R02, R03, R04, R05, \
              R10, R11, R12, R13, R14, R15, \
              R20, R21, R22, R23, R24, R25, \
              R30, R31, R32, R33, R34, R35, \
              R40, R41, R42, R43, R44, R45  \
    )                                       \
    {                                       \
        { L00, L01, L02, L03, L04, L05 },   \
        { L10, L11, L12, L13, L14, L15 },   \
        { L20, L21, L22, L23, L24, L25 },   \
        { L30, L31, L32, L33, L34, L35 },   \
        { L40, L41, L42, L43, L44, L45 },   \
                                            \
        { M00, M01, M02 },                  \
        { M10, M11, M12 },                  \
        { M20, M21, M22 },                  \
        { M30, M31, M32 },                  \
        { M40, M41, M42 },                  \
                                            \
        { R00, R01, R02, R03, R04, R05 },   \
        { R10, R11, R12, R13, R14, R15 },   \
        { R20, R21, R22, R23, R24, R25 },   \
        { R30, R31, R32, R33, R34, R35 },   \
        { R40, R41, R42, R43, R44, R45 }   \
    }
