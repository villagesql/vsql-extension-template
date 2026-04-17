/* Copyright (c) 2025 VillageSQL Contributors
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <https://www.gnu.org/licenses/>.
 */

#include <villagesql/extension.h>

#include <cstring>

using namespace villagesql::extension_builder;
using namespace villagesql::func_builder;

// Hello world function implementation
void hello_world_impl(vef_context_t* ctx, vef_vdf_result_t* result) {
  const char* hello = "Hello, World!";
  strcpy(result->str_buf, hello);
  result->type = VEF_RESULT_VALUE;
  result->actual_len = strlen(hello);
}

// Extension registration
VEF_GENERATE_ENTRY_POINTS(
  make_extension("vsql_extension_template", "1.0.0")
    .func(make_func<&hello_world_impl>("hello_world")
      .returns(STRING)
      .buffer_size(14)  // "Hello, World!" + null terminator
      .build())
)
