//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : cmake
// Filename    : cmake/templates/projectversion.h.in
// Created by  : Steinberg, 03/2022
// Description : cmake template file to generate the projectversion.h 
//
//-----------------------------------------------------------------------------
// LICENSE
// (c) 2022, Steinberg Media Technologies GmbH, All Rights Reserved
//-----------------------------------------------------------------------------
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// 
//   * Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation 
//     and/or other materials provided with the distribution.
//   * Neither the name of the Steinberg Media Technologies nor the names of its
//     contributors may be used to endorse or promote products derived from this 
//     software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#pragma once

#define MAJOR_VERSION_STR "3"
#define MAJOR_VERSION_INT  3

#define SUB_VERSION_STR "7"
#define SUB_VERSION_INT 7

#define RELEASE_NUMBER_STR "14"
#define RELEASE_NUMBER_INT 14

#define BUILD_NUMBER_STR "0" // Build number to be sure that each result could identified.
#define BUILD_NUMBER_INT 0

// Version possibly with build number (example "1.0.3.342")
#define FULL_VERSION_STR "3.7.14.0"

// Version without build number (example "1.0.3")
#define VERSION_STR  "3.7.14"
