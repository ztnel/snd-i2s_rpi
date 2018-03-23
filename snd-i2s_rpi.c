/*
 * =====================================================================================
 *
 *       Filename:  snd-i2s_rpi
 *
 *    Description:  
 *
 *        Version:  0.0.2
 *        Created:  2018-03-23
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Paul Creaser (), Huan Truong (htruong@tnhh.net)
 *   Organization:  Crankshaft
 *
 * =====================================================================================
 */
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/kmod.h>
#include <linux/platform_device.h>
#include <sound/simple_card.h>
#include <linux/delay.h>
#include "snd-i2s_rpi.h"

/*
 * modified for linux 4.1.5
 * inspired by https://github.com/msperl/spi-config
 * with thanks for https://github.com/notro/rpi-source/wiki
 * as well as Florian Meier for the rpi i2s and dma drivers
 *
 * to use a differant (simple-card compatible) codec
 * change the codec name string in two places and the
 * codec_dai name string. (see codec's source file)
 *
 *
 * N.B. playback vs capture is determined by the codec choice
 * */

void device_release_callback(struct device *dev) { /*  do nothing */ };

static short rpi_platform_generation = 1;
module_param(rpi_platform_generation, short, 0);
MODULE_PARM_DESC(rpi_platform_generation, "Raspberry Pi generation: 0=BCM2835, 1=Others");

static struct asoc_simple_card_info snd_rpi_simple_card_info = {
	.card = "snd_rpi_i2s_card", // -> snd_soc_card.name
	.name = "simple-card_codec_link", // -> snd_soc_dai_link.name
	.codec = "snd-soc-dummy", // "dmic-codec", // -> snd_soc_dai_link.codec_name
	.platform = "3f203000.i2s",
	.daifmt = SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_NB_NF | SND_SOC_DAIFMT_CBS_CFS,
	.cpu_dai = {
		.name = "3f203000.i2s", // -> snd_soc_dai_link.cpu_dai_name
		.sysclk = 0 },
	.codec_dai = {
	.name = "snd-soc-dummy-dai", //"dmic-codec", // -> snd_soc_dai_link.codec_dai_name
	.sysclk = 0 },
};

static struct platform_device snd_rpi_simple_card_device = {
	.name = "asoc-simple-card", //module alias
	.id = 0,
	.num_resources = 0,
	.dev = {
		.release = &device_release_callback,
		.platform_data = &snd_rpi_simple_card_info, // *HACK ALERT*
	},
};

int i2s_rpi_init(void)
{
	const char *dmaengine = "bcm2708-dmaengine"; //module name
	struct platform_device snd_card;
	int ret;

	printk(KERN_INFO "snd-i2s_rpi: Version %s\n", SND_I2S_RPI_VERSION);

	ret = request_module(dmaengine);
	// pr_alert("request module load '%s': %d\n",dmaengine, ret);

	snd_card = snd_rpi_simple_card_device;

	if (rpi_platform_generation == 0) {
		struct asoc_simple_card_info card_info_alt;

		card_info_alt = snd_rpi_simple_card_info;
		card_info_alt.platform = "20203000.i2s";
		card_info_alt.cpu_dai.name = "20203000.i2s";

		snd_card.dev.platform_data = &card_info_alt;
	}

	ret = platform_device_register(&snd_card);



	//pr_alert("register platform device '%s': %d\n",snd_rpi_simple_card_device.name, ret);

	return 0;
}

void i2s_rpi_exit(void)
{
	// you'll have to sudo modprobe -r the card & codec drivers manually (first?)
	platform_device_unregister(&snd_rpi_simple_card_device);
	//pr_alert("i2s mic module unloaded\n");
}


module_init(i2s_rpi_init);
module_exit(i2s_rpi_exit);
MODULE_DESCRIPTION("ASoC simple-card I2S Microphone");
MODULE_AUTHOR("Paul Creaser");
MODULE_LICENSE("GPL v2");

