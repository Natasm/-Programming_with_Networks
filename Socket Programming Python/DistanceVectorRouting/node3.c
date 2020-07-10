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
} dt3;

/* students to write the following two routines, and maybe some others */

void rtinit3()
{
    for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			dt3.costs[i][j] = 999;

	dt3.costs[3][0] = 7;
	dt3.costs[3][2] = 2;
	dt3.costs[3][3] = 0;

	printdt3(&dt3);

	struct rtpkt pkt0, pkt2;
	pkt0.sourceid = 3;
	pkt0.destid = 0;
	pkt2.sourceid = 3;
	pkt2.destid = 2;

	for (int i = 0; i < 4; i++)
	{
		pkt0.mincost[i] = dt3.costs[3][i];
		pkt2.mincost[i] = dt3.costs[3][i];
	}

	tolayer2(pkt0);
	tolayer2(pkt2);
}


void rtupdate3(rcvdpkt)
  struct rtpkt *rcvdpkt;

{
	if (rcvdpkt->destid != 3) return;

	int j = rcvdpkt->sourceid;
	int flag = 0;

	printf("\nPacket rcvd at node 3 from node %d \n", j);

	for (int i = 0; i < 4; i++)
	{
		if (dt3.costs[j][i] > dt3.costs[3][j] + rcvdpkt->mincost[i])
		{
			dt3.costs[j][i] = dt3.costs[3][j] + rcvdpkt->mincost[i];
			if(i != 3) flag = 1;
		}
	}

	if (flag == 1) //change in dt3 detected!
	{
		printf("\nDistance table at node 3 updated: \n");
		printdt3(&dt3);

		struct rtpkt pkt0, pkt2;
		pkt0.sourceid = 3;
		pkt0.destid = 0;
		pkt2.sourceid = 3;
		pkt2.destid = 2;
		pkt0.mincost[3] = 0;
		pkt2.mincost[3] = 0;

		for (int i = 0; i < 4; i++)
		{
			if (i == 3) continue;
			pkt0.mincost[i] = min(dt3.costs[0][i], dt3.costs[1][i], dt3.costs[2][i]);
			pkt2.mincost[i] = min(dt3.costs[0][i], dt3.costs[1][i], dt3.costs[2][i]);
		}

		printf("\nSending routing packets to nodes 0 and 2 with following mincost vector: \n");
		for (int i = 0; i < 4; i++)
			printf("%d\t", pkt0.mincost[i]);
		printf("\n");

		tolayer2(pkt0);
		tolayer2(pkt2);
	}
}


printdt3(dtptr)
  struct distance_table *dtptr;

{
  printf("             via     \n");
  printf("   D3 |    0     2 \n");
  printf("  ----|-----------\n");
  printf("     0|  %3d   %3d\n",dtptr->costs[0][0], dtptr->costs[0][2]);
  printf("dest 1|  %3d   %3d\n",dtptr->costs[1][0], dtptr->costs[1][2]);
  printf("     2|  %3d   %3d\n",dtptr->costs[2][0], dtptr->costs[2][2]);

}
