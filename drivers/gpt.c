#include <imx_gpt.h>
#include <gpt.h>
#include "int.h"
#include "imx6ul.h" 

static uint32_t gpt_tick(imx_gpt_t *gpt)
{
    return gpt->cnt;
}

void udelay(uint32_t us)
{
    imx_gpt_t *gpt = (imx_gpt_t *)0x02098000UL;
    uint32_t delay_ticks = 0;
    uint32_t start_ticks = 0;
    start_ticks = gpt_tick(gpt);
    delay_ticks = us;
    while ((gpt_tick(gpt) - start_ticks) < delay_ticks);
}

void mdelay(uint32_t ms)
{
    udelay(ms * 1000);
}

void gpt_init()
{
    imx_gpt_t *gpt = (imx_gpt_t *)0x02098000UL;

    gpt->sr = 0x0000003F;
    gpt->cr &= ~GPT_CR_EN_MASK;
    gpt->cr = GPT_CR_ENMOD_MASK | GPT_CR_CLKSRC_24M;

    gpt->pr = (7UL << GPT_PR_PRESCALER24M_SHIFT) | (15UL << GPT_PR_PRESCALER_SHIFT);
    gpt->cr |= (GPT_CR_EN24M_MASK | GPT_CR_FRR_MASK);

    gpt->cr |= GPT_CR_EN_MASK;
}
