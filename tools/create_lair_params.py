#!/usr/bin/env python3
"""
Catalogue all Create_Lair() callsites and compute the UU and budget
parameter ranges for each.

Create_Lair(lair_idx, wp, wx, wy, UU, lair_type, budget)
                                   ^^              ^^^^^^
"""

def clamp(val, lo, hi):
    return max(lo, min(hi, val))

print("=" * 120)
print("Create_Lair() Callsite Parameter Analysis")
print("=" * 120)


# --- Callsite 1: Towers ---------------------------------------------
# Create_Lair(itr, ARCANUS_PLANE, wx, wy, 3, 0, (650 + (50 * Random(11))));
# Random(11) returns 1..11
print()
print("-" * 120)
print("CALLSITE 1: Towers  (indices 0..NUM_TOWERS-1 = 0..5)")
print("  lair_type = 0 (lt_Tower)")
print("  UU        = 3 (hardcoded)")
print()
print(f"  {'Random(11)':>12}  {'UU':>6}  {'budget':>8}  {'budget formula'}")
print(f"  {'-'*12}  {'-'*6}  {'-'*8}  {'-'*30}")
for r in range(1, 12):
    budget = 650 + (50 * r)
    print(f"  {r:>12}  {3:>6}  {budget:>8}  650 + (50 * {r})")
print(f"\n  UU range:     3 (constant)")
print(f"  budget range: {650 + 50*1} .. {650 + 50*11}")


# --- Callsite 1b: Tower Mirrors -------------------------------------
print()
print("-" * 120)
print("CALLSITE 1b: Tower Mirrors  (indices NUM_TOWERS..2*NUM_TOWERS-1 = 6..11)")
print("  memcpy from Arcanus tower, then set wp = MYRROR_PLANE")
print("  (not a Create_Lair call -- inherits UU/budget from the Arcanus tower)")


# --- Callsite 2: Nodes ----------------------------------------------
# niu_generate_lair_value = (((((_NODES[itr].power - 1) * _magic) / 2) / 6) + 1);
# SETMAX(niu_generate_lair_value, 4);
# SETMIN(niu_generate_lair_value, 1);
# Create_Lair(..., niu_generate_lair_value, (_NODES[itr].type + 1),
#     ((((_NODES[itr].power * _NODES[itr].power) * (4 + Random(11))) * (_magic + 1)) / 2));
# Random(11) returns 1..11
# _magic is typically 1..5 (from game settings)
# _NODES[itr].power is typically 6..11 (from SetUp in test, but original range?)
print()
print("-" * 120)
print("CALLSITE 2: Nodes  (indices 2*NUM_TOWERS..2*NUM_TOWERS+NUM_NODES-1 = 12..41)")
print("  lair_type = (_NODES[itr].type + 1)  ->  1=Sorcery, 2=Nature, 3=Chaos")
print()
print("  UU formula:     clamp( (((power-1) * magic / 2) / 6) + 1,  1, 4 )")
print("  budget formula: (power² * (4 + Random(11)) * (magic + 1)) / 2")
print()
print(f"  {'power':>6}  {'magic':>6}  {'UU':>4}  {'budget_min':>12}  {'budget_max':>12}  {'UU formula detail'}")
print(f"  {'-'*6}  {'-'*6}  {'-'*4}  {'-'*12}  {'-'*12}  {'-'*40}")
for power in range(1, 16):
    for magic in [1, 2, 3, 4, 5]:
        # UU: integer division throughout (C int16_t)
        uu_raw = (((power - 1) * magic) // 2) // 6 + 1
        uu = clamp(uu_raw, 1, 4)
        # budget: Random(11) returns 1..11
        budget_min = ((power * power) * (4 + 1) * (magic + 1)) // 2
        budget_max = ((power * power) * (4 + 11) * (magic + 1)) // 2
        detail = f"((({power}-1)*{magic}/2)/6)+1 = {uu_raw} -> clamped {uu}"
        print(f"  {power:>6}  {magic:>6}  {uu:>4}  {budget_min:>12}  {budget_max:>12}  {detail}")


# --- Callsite 3: Strong Lairs ---------------------------------------
# if(wp == ARCANUS_PLANE)  generate_lair_budget_value = (50 + (Random(29) * 50));
# else                     generate_lair_budget_value = (100 + (Random(24) * 100));
# niu_generate_lair_value = generate_lair_budget_value;    /* bullshit? */
# Create_Lair(..., niu_generate_lair_value, 4, generate_lair_budget_value);
# Random(29) returns 1..29;  Random(24) returns 1..24
print()
print("-" * 120)
print("CALLSITE 3: Strong Lairs  (indices 2*NUM_TOWERS+NUM_NODES..+24 = 42..66)")
print("  lair_type = 4 (random encounter)")
print("  *** UU = budget (same value passed for both) ***")
print()
print("  Arcanus:")
print(f"    Random(29)  UU=budget range: {50 + 1*50} .. {50 + 29*50}")
for r in [1, 5, 10, 15, 20, 25, 29]:
    val = 50 + (r * 50)
    print(f"      Random={r:>2}  ->  UU=budget={val}")
print()
print("  Myrror:")
print(f"    Random(24)  UU=budget range: {100 + 1*100} .. {100 + 24*100}")
for r in [1, 5, 10, 15, 20, 24]:
    val = 100 + (r * 100)
    print(f"      Random={r:>2}  ->  UU=budget={val}")


# --- Callsite 4: Weak Lairs -----------------------------------------
# if(wp == ARCANUS_PLANE)  generate_lair_budget_value = (Random(10) * 10);
# else                     generate_lair_budget_value = (Random(20) * 10);
# niu_generate_lair_value = generate_lair_budget_value;
# Create_Lair(..., niu_generate_lair_value, 4, generate_lair_budget_value);
# Random(10) returns 1..10;  Random(20) returns 1..20
print()
print("-" * 120)
print("CALLSITE 4: Weak Lairs  (indices 42+25..42+25+31 = 67..98)")
print("  lair_type = 4 (random encounter)")
print("  *** UU = budget (same value passed for both) ***")
print()
print("  Arcanus:")
print(f"    Random(10)  UU=budget range: {1*10} .. {10*10}")
for r in range(1, 11):
    val = r * 10
    print(f"      Random={r:>2}  ->  UU=budget={val}")
print()
print("  Myrror:")
print(f"    Random(20)  UU=budget range: {1*10} .. {20*10}")
for r in [1, 5, 10, 15, 20]:
    val = r * 10
    print(f"      Random={r:>2}  ->  UU=budget={val}")


# --- Summary --------------------------------------------------------
print()
print("=" * 120)
print("SUMMARY")
print("=" * 120)
print()
print(f"  {'Callsite':<20}  {'lair_type':<12}  {'UU range':<16}  {'budget range':<20}  {'UU == budget?'}")
print(f"  {'-'*20}  {'-'*12}  {'-'*16}  {'-'*20}  {'-'*14}")
print(f"  {'Towers':<20}  {'0':<12}  {'3':<16}  {'700..1200':<20}  {'NO'}")
print(f"  {'Tower Mirrors':<20}  {'(copied)':<12}  {'(copied)':<16}  {'(copied)':<20}  {'(copied)'}")
print(f"  {'Nodes':<20}  {'1-3':<12}  {'1..4':<16}  {'5..12375':<20}  {'NO'}")
print(f"  {'Strong Lairs':<20}  {'4':<12}  {'100..1500':<16}  {'100..2500':<20}  {'YES <- suspect'}")
print(f"  {'Weak Lairs':<20}  {'4':<12}  {'10..100':<16}  {'10..200':<20}  {'YES <- suspect'}")
print()
print("Observation: UU is always a small bounded integer (1-4) when independently")
print("computed (towers, nodes). For random lairs, UU is set to the budget value")
print("(large, unbounded), suggesting the random-lair callsites are passing the")
print("wrong value for UU.")
print()
print("Inside Create_Lair(), the UU parameter is currently UNUSED -- never read.")
print("This may indicate that the original Create_Lair had UU-dependent logic")
print("that was lost during reverse-engineering.")
