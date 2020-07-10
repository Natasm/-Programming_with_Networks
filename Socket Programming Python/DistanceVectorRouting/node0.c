#include <stdio.h>

extern struct rtpkt {
  int sourceid;       /* id of sending router sending this pkt */
  int destid;         /* id of router to which pkt being sent
                         (must be an immediate neighbor) */
  int mincost[4];    /* min cost to node 0 ... 3 */
  };

extern int TRACE;
extern int YES;
extern int NO;

struct distance_table
{
  int costs[4][4];
} dt0;

int min(int a, int b, int c)
{
	int temp = (a <= b ? a : b);
	temp = (temp <= c ? temp : c);
	return temp;

}

/* students to write the following two routines, and maybe some others */

void rtinit0()
{
    for(int i = 0; i < 4; i++)
        for(int j = 0; i < 4; i++)
             dt0.costs[i][j] = 999;

    dt0.costs[0][0] = 0;
    dt0.costs[0][1] = 1;
    dt0.costs[0][2] = 3;
    dt0.costs[0][3] = 7;

    printdt0(&dt0);

    struct rtpkt pkt1, pkt2, pkt3;
	pkt1.sourceid = 0;
	pkt1.destid = 1;
	pkt2.sourceid = 0;
	pkt2.destid = 2;
	pkt3.sourceid = 0;
	pkt3.destid = 3;

	for (int i = 0; i < 4; i++)
	{
		pkt1.mincost[i] = dt0.costs[0][i];
		pkt2.mincost[i] = dt0.costs[0][i];
		pkt3.mincost[i] = dt0.costs[0][i];
	}

	tolayer2(pkt1);
	tolayer2(pkt2);
	tolayer2(pkt3);
}


void rtupdate0(rcvdpkt)
  struct rtpkt *rcvdpkt;
{
	if (rcvdpkt->destid != 0) return;

	int j = rcvdpkt->sourceid;
	int flag = 0;

	printf("\nPacket rcvd at node 0 from node %d \n", j);

	for (int i = 0; i < 4; i++)
	{
		if (dt0.costs[j][i] > dt0.costs[j][0]+ rcvdpkt->mincost[i])
		{
			dt0.costs[j][i] = dt0.costs[0][j] + rcvdpkt->mincost[i];
			if(i != 0) flag = 1;
		}
	}
	if (flag == 1) //change in dt0 detected!
	{
		printf("\nDistance table at node 0 updated: \n");
		printdt0(&dt0);

		struct rtpkt pkt1, pkt2, pkt3;
		pkt1.sourceid = 0;
		pkt1.destid = 1;
		pkt2.sourceid = 0;
		pkt2.destid = 2;
		pkt3.sourceid = 0;
		pkt3.destid = 3;
		pkt1.mincost[0] = 0;
		pkt2.mincost[0] = 0;
		pkt3.mincost[0] = 0;

		for (int i = 1; i < 4; i++)
		{
			pkt1.mincost[i] = min(dt0.costs[1][i], dt0.costs[2][i], dt0.costs[3][i]);
			pkt2.mincost[i] = min(dt0.costs[1][i], dt0.costs[2][i], dt0.costs[3][i]);
			pkt3.mincost[i] = min(dt0.costs[1][i], dt0.costs[2][i], dt0.costs[3][i]);

		}

		printf("\nSending routing packets to nodes 1, 2 and 3 with following mincost vector: \n");
		for (int i = 0; i < 4; i++)
			printf("%d\t", pkt1.mincost[i]);
		printf("\n");

		tolayer2(pkt1);
		tolayer2(pkt2);
		tolayer2(pkt3);
	}
}


printdt0(dtptr)
  struct distance_table *dtptr;

{
  printf("                via     \n");
  printf("   D0 |    1     2    3 \n");
  printf("  ----|-----------------\n");
  printf("     1|  %3d   %3d   %3d\n",dtptr->costs[1][1],
	 dtptr->costs[1][2],dtptr->costs[1][3]);
  printf("dest 2|  %3d   %3d   %3d\n",dtptr->costs[2][1],
	 dtptr->costs[2][2],dtptr->costs[2][3]);
  printf("     3|  %3d   %3d   %3d\n",dtptr->costs[3][1],
	 dtptr->costs[3][2],dtptr->costs[3][3]);
}

linkhandler0(linkid, newcost)
  int linkid, newcost;

/* called when cost from 0 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */

{
}
