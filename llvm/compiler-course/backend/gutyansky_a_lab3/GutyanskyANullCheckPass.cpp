#include "MCTargetDesc/X86BaseInfo.h"
#include "MCTargetDesc/X86MCTargetDesc.h"
#include "X86.h"
#include "X86InstrInfo.h"
#include "X86Subtarget.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/Register.h"
#include "llvm/IR/DebugLoc.h"

using namespace llvm;

namespace {
class GutyanskyANullCheckPass : public MachineFunctionPass {
public:
  static char ID;
  GutyanskyANullCheckPass() : MachineFunctionPass(ID) {}
  bool runOnMachineFunction(MachineFunction &Func) override;

private:
  static void addCheck(MachineFunction &MF, MachineBasicBlock &BasicBlock,
                       MachineInstr &Insn, Register Reg);
  static bool shouldCheckRegister(Register Reg);
};

char GutyanskyANullCheckPass::ID = 0;

bool GutyanskyANullCheckPass::shouldCheckRegister(Register Reg) {
  switch (Reg) {
  case X86::ESP:
  case X86::EBP:
  case X86::RSP:
  case X86::RBP:
    return false;
  default:
    return true;
  }
}

void GutyanskyANullCheckPass::addCheck(MachineFunction &MF,
                                       MachineBasicBlock &BasicBlock,
                                       MachineInstr &Insn, Register Reg) {
  const X86InstrInfo *InstrInfo =
      MF.getSubtarget<X86Subtarget>().getInstrInfo();
  DebugLoc DLoc = Insn.getDebugLoc();

  BuildMI(BasicBlock, Insn, DLoc, InstrInfo->get(TargetOpcode::COPY), X86::RDI)
      .addReg(Reg);
  BuildMI(BasicBlock, Insn, DLoc, InstrInfo->get(X86::CALL64pcrel32))
      .addExternalSymbol("check_null");
}

bool GutyanskyANullCheckPass::runOnMachineFunction(MachineFunction &Func) {
  bool HasChanges = false;

  for (auto &BasicBlock : Func) {
    for (auto &Insn : BasicBlock) {

      if (!Insn.mayLoadOrStore())
        continue;

      const auto &Desc = Insn.getDesc();

      int OpNo = X86II::getMemoryOperandNo(Desc.TSFlags);
      if (OpNo == -1)
        continue;

      OpNo += X86II::getOperandBias(Desc);

      const auto &Operand = Insn.getOperand(OpNo + X86::AddrBaseReg);
      if (!Operand.isReg() || !shouldCheckRegister(Operand.getReg()))
        continue;

      addCheck(Func, BasicBlock, Insn, Operand.getReg());

      HasChanges = true;
    }
  }

  return HasChanges;
}
} // namespace

static RegisterPass<GutyanskyANullCheckPass>
    X("gutyansky-a-null-check-x86", "Insert null pointer checks", false, false);
