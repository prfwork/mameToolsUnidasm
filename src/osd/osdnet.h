// license:BSD-3-Clause
// copyright-holders:Carl
#ifndef MAME_OSD_OSDNET_H
#define MAME_OSD_OSDNET_H

#pragma once

#include <algorithm>

class osd_netdev;

#define CREATE_NETDEV(name) class osd_netdev *name(const char *ifname, class device_network_interface *ifdev, int rate)
typedef class osd_netdev *(*create_netdev)(const char *ifname, class device_network_interface *ifdev, int rate);

class osd_netdev
{
public:
	struct entry_t
	{
		entry_t()
		{
			std::fill(std::begin(name), std::end(name), 0);
			std::fill(std::begin(description), std::end(description), 0);
		}

		int id = 0;
		char name[256];
		char description[256];
		create_netdev func = nullptr;
	};
	osd_netdev(class device_network_interface *ifdev, int rate);
	virtual ~osd_netdev();
	void start();
	void stop();

	virtual int send(uint8_t *buf, int len);
	virtual void set_mac(const char *mac);
	virtual void set_promisc(bool promisc);

	const char *get_mac();
	bool get_promisc();

protected:
	virtual int recv_dev(uint8_t **buf);

private:
	void recv(int32_t param);

	class device_network_interface *m_dev;
	emu_timer *m_timer;
};

class osd_netdev *open_netdev(int id, class device_network_interface *ifdev, int rate);
void add_netdev(const char *name, const char *description, create_netdev func);
void clear_netdev();
const std::vector<std::unique_ptr<osd_netdev::entry_t>>& get_netdev_list();
int netdev_count();
void osd_list_network_adapters();

#endif // MAME_OSD_OSDNET_H
