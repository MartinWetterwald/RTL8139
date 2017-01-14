#include "net.h"
#include <linux/pci.h>

// r8139dn_ops stores fonctors to our driver actions,
// so that the kernel can call the relevant one when needed
struct net_device_ops r8139dn_ops = { };

struct net_device * r8139dn_net_init ( struct pci_dev * pdev )
{
    struct net_device * ndev;
    struct r8139dn_priv * priv;

    // Allocate a eth device
    ndev = alloc_etherdev ( sizeof ( * priv ) );
    if ( ! ndev )
    {
        return ERR_PTR ( -ENOMEM );
    }

    priv = netdev_priv ( ndev );
    priv -> pdev = pdev;

    ndev -> netdev_ops = & r8139dn_ops;

    // Add our net device as a leaf to our PCI bus in /sys tree
    SET_NETDEV_DEV ( ndev, & ( pdev -> dev ) );

    // From the PCI device, we want to be able to retrieve our network device
    // So we store it. Later we can retrieve it with pci_get_drvdata
    pci_set_drvdata ( pdev, ndev );

    return ndev;
}