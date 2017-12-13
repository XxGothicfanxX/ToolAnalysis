// Class that represents a full readout from all of the DAQ VME
// cards. Includes data for a single trigger in non-Hefty mode
// or multiple triggers in Hefty mode.
//
// Steven Gardiner <sjgardiner@ucdavis.edu>
#pragma once

// standard library includes
#include <map>

// reco-annie includes
#include "Constants.hh"
#include "RawCard.hh"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>

namespace annie {

  class RawReadout {

    public:

      RawReadout(int SequenceID = BOGUS_INT) : sequence_id_(SequenceID) {}

      inline void set_sequence_id(int seq_id) { sequence_id_ = seq_id; }
      inline int sequence_id() const { return sequence_id_; }

      void add_card(int CardID, unsigned long long LastSync, int StartTimeSec,
        int StartTimeNSec, unsigned long long StartCount, int Channels,
        int BufferSize, int MiniBufferSize,
        const std::vector<unsigned short>& FullBufferData,
        const std::vector<unsigned long long>& TriggerCounts,
        const std::vector<unsigned int>& Rates, bool overwrite_ok = false);

      inline const std::map<int, annie::RawCard> cards() const
        { return cards_; }
      inline const annie::RawCard& card(int index) const
        { return cards_.at(index); }

      inline const annie::RawChannel& channel(int card_index,
        int channel_index)
      {
        return cards_.at(card_index).channel(channel_index);
      }

    protected:

      /// @brief Integer index identifying this DAQ readout (unique within
      /// a run)
      int sequence_id_;

      /// @brief Raw data for each of the VME cards included in the readout
      /// @details Keys are VME card IDs, values are RawCard objects storing
      /// the associated data from the PMTData tree.
      std::map<int, annie::RawCard> cards_;

  private:

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
      ar & sequence_id_;
      ar & cards_;
    }

  };
}
