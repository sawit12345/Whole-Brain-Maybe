#pragma once
#include <cstdint>
#include <vector>
#include "../common/bitvec.hpp"
#include "../common/rng.hpp"

namespace omega {

static constexpr unsigned N_BITS = 2048;
static constexpr unsigned K_PER_PLANE = 20;
static constexpr unsigned N_WORDS = N_BITS >> 6;
static constexpr unsigned INPUT_BITS_LOG2 = 12;
static constexpr unsigned EXP_BITS_LOG2 = 13;

struct Code {
    BitVec pos{N_BITS};
    BitVec neg{N_BITS};
};

uint32_t sim(const Code& a, const Code& b);
bool invariant_ok(const Code& c);
uint64_t mix_streams(uint64_t stream_id, uint64_t tag);
inline uint64_t branch_seed(uint64_t stream_id, unsigned t) {
    return mix_streams((stream_id << 1) + stream_id + t,
                       0x5BD1E9955BD1E995ull);
}
Code encode(uint64_t stream_id, Rng& rng);
Code bind(const Code& x, const Code& role);
Code unbind(const Code& y, const Code& role);

struct BddMemory {
    static constexpr unsigned BRANCHES = 3;
    static constexpr int32_t DEFAULT_THRESHOLD =
        K_PER_PLANE + K_PER_PLANE + K_PER_PLANE;
    explicit BddMemory(unsigned capacity_slots);
    bool store(uint64_t stream_id);
    bool recall(const Code& q, uint64_t& out_stream);
    void strengthen(uint64_t stream_id);
    void decay_all();
    void set_threshold(int32_t t) { threshold_ = t; }
    uint8_t conf(uint64_t stream_id, unsigned branch) const;
    struct Stats {
        unsigned capacity;
        unsigned slots_used;
        unsigned recalls;
        unsigned hits;
        unsigned misses;
        int32_t threshold;
    };
    Stats stats() const;

private:
    struct Slot {
        uint64_t stream = 0;
        bool used = false;
        uint8_t conf_bits[BRANCHES] = {};
        Code branch[BRANCHES];
    };
    static uint8_t inc2(uint8_t c) {
        return static_cast<uint8_t>(c + (((c + 1u) >> 2) ^ 1u));
    }
    static uint8_t dec2(uint8_t c) {
        return static_cast<uint8_t>(c - (((c + 3u) >> 2) & 1u));
    }
    std::vector<Slot> slots_;
    int32_t threshold_ = DEFAULT_THRESHOLD;
    unsigned recalls_ = 0;
    unsigned hits_ = 0;
    unsigned misses_ = 0;
};

struct Router {
    static constexpr unsigned MAX_AREAS = 64;
    Router(unsigned num_areas, unsigned fanout);
    void set_gate(unsigned area_i, unsigned area_j, bool on);
    bool gate(unsigned area_i, unsigned area_j) const;
    uint64_t gate_word(unsigned area_i) const { return masks_[area_i]; }
    unsigned route(const Code& x, unsigned* out_area,
                   uint32_t* out_score) const;

private:
    unsigned n_;
    unsigned k_;
    uint64_t masks_[MAX_AREAS];
};

struct GridClock {
    static constexpr uint64_t COMPOSITE_PERIOD = 1944497ull;
    GridClock();
    void tick();
    bool cycle() const;
    bool phase(unsigned which) const;

private:
    uint16_t remain_[3];
    bool phase_[3];
};

struct Cerebellum {
    static constexpr unsigned EXP_BITS = 8192;
    static constexpr unsigned HASH_WORDS = N_WORDS + N_WORDS;
    static constexpr unsigned GRANULE_FANIN = 12;
    Cerebellum();
    void recode(const Code& in, BitVec& out) const;
    int predict(const Code& in) const;
    void train(const Code& in, int label);
    int8_t weight(unsigned j) const { return w_[j]; }

private:
    std::vector<uint64_t> hashes_;
    std::vector<int8_t> w_;
};

struct Selector {
    static constexpr unsigned MAX_CHANNELS = 16;
    explicit Selector(unsigned num_channels);
    void set_score(unsigned channel, uint32_t s) { scores_[channel] = s; }
    uint32_t score(unsigned channel) const { return scores_[channel]; }
    unsigned select() const;
    void set_plasticity(bool on) { plasticity_ = on; }
    bool plasticity_on() const { return plasticity_; }

private:
    unsigned n_;
    uint32_t scores_[MAX_CHANNELS] = {};
    bool plasticity_ = false;
};

struct ModBus {
    int8_t da = 0;
    int8_t ach = 0;
    int8_t ne = 0;
    int8_t s5ht = 0;
    void apply(int& threshold) const;
};

}
