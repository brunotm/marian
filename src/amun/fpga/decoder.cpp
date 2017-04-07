#include "decoder.h"

namespace amunmt {
namespace FPGA {

void Decoder::EmptyState(mblas::Matrix& State,
                const mblas::Matrix& SourceContext,
                size_t batchSize,
                const Array<int>& batchMapping)
{
  rnn1_.InitializeState(State, SourceContext, batchSize, batchMapping);
  alignment_.Init(SourceContext);
}

void Decoder::EmptyEmbedding(mblas::Matrix& Embedding, size_t batchSize) {
  Embedding.Resize(batchSize, embeddings_.GetCols());
  mblas::Fill(Embedding, 0);
}

void Decoder::Decode(mblas::Matrix& NextState,
              const mblas::Matrix& State,
              const mblas::Matrix& Embeddings,
              const mblas::Matrix& SourceContext,
              const Array<int>& mapping,
              const std::vector<size_t>& beamSizes)
{
  std::cerr << std::endl;

  std::cerr << "1HiddenState_=" << HiddenState_.Debug(1) << std::endl;
  std::cerr << "State=" << State.Debug(1) << std::endl;
  std::cerr << "Embeddings=" << Embeddings.Debug(1) << std::endl;
  GetHiddenState(HiddenState_, State, Embeddings);
  std::cerr << "2HiddenState_=" << HiddenState_.Debug(1) << std::endl;

  GetAlignedSourceContext(AlignedSourceContext_, HiddenState_, SourceContext, mapping, beamSizes);
  std::cerr << "AlignedSourceContext_=" << AlignedSourceContext_.Debug(1) << std::endl;

}

void Decoder::GetHiddenState(mblas::Matrix& HiddenState,
                    const mblas::Matrix& PrevState,
                    const mblas::Matrix& Embedding) {
  rnn1_.GetNextState(HiddenState, PrevState, Embedding);
}

void Decoder::GetAlignedSourceContext(mblas::Matrix& AlignedSourceContext,
                              const mblas::Matrix& HiddenState,
                              const mblas::Matrix& SourceContext,
                              const Array<int>& mapping,
                              const std::vector<size_t>& beamSizes)
{
  alignment_.GetAlignedSourceContext(AlignedSourceContext, HiddenState, SourceContext,
                                     mapping, beamSizes);

}


} // namespace
}

