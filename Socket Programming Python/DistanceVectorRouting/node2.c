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
} dt2;


/* students to write the following two routines, and maybe some others */

void rtinit2()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			dt2.costs[i][j] = 999;

	dt2.costs[2][0] = 3;
	dt2.costs[2][1] = 1;
	dt2.costs[2][2] = 0;
	dt2.costs[2][3] = 2;

	printdt2(&dt2);

	struct rtpkt pkt0, pkt1, pkt3;
	pkt0.sourceid = 2;
	pkt0.destid = 0;
	pkt1.sourceid = 2;
	pkt1.destid = 1;
	pkt3.sourceid = 2;
	pkt3.destid = 3;

	for (int i = 0; i < 4; i++)
	{
		pkt0.mincost[i] = dt2.costs[2][i];
		pkt1.mincost[i] = dt2.costs[2][i];
		pkt3.mincost[i] = dt2.costs[2][i];
	}

	tolayer2(pkt0);
	tolayer2(pkt1);
	tolayer2(pkt3);
}


void rtupdate2(rcvdpkt)
  struct rtpkt *rcvdpkt;

{
	if (rcvdpkt->destid != 2) return;

	int j = rcvdpkt->sourceid;
	int flag = 0;

	printf("\nPacket rcvd at node 2 from node %d \n", j);

	for (int i = 0; i < 4; i++)
	{
		if (dt2.costs[j][i] > dt2.costs[2][j] + rcvdpkt->mincost[i])
		{
			dt2.costs[j][i] = dt2.costs[2][j] + rcvdpkt->mincost[i];
			if(i != 2) flag = 1;
		}
	}

	if (flag == 1) //change in dt2 detected!
	{
		printf("\nDistance table at node 2 updated: \n");
		printdt2(&dt2);

		struct rtpkt pkt0, pkt1, pkt3;
		pkt0.sourceid = 2;
		pkt0.destid = 0;
		pkt1.sourceid = 2;
		pkt1.destid = 1;
		pkt3.sourceid = 2;
		pkt3.destid = 3;
		pkt0.mincost[2] = 0;
		pkt1.mincost[2] = 0;
		pkt3.mincost[2] = 0;

		for (int i = 0; i < 4; i++)
		{
			if (i == 2) continue;
			pkt0.mincost[i] = min(dt2.costs[0][i], dt2.costs[1][i], dt2.costs[3][i]);
			pkt1.mincost[i] = min(dt2.costs[0][i], dt2.costs[1][i], dt2.costs[3][i]);
			pkt3.mincost[i] = min(dt2.costs[0][i], dt2.costs[1][i], dt2.costs[3][i]);

		}

		printf("\nSending routing packets to nodes 0, 1 and 3 with following mincost vector: \n");
		for (int i = 0; i < 4; i++)
			printf("%d\t", pkt0.mincost[i]);
		printf("\n");

		tolayer2(pkt0);
		tolayer2(pkt1);
		tolayer2(pkt3);
	}
}


printdt2(dtptr)
  struct distance_table *dtptr;

{
  printf("                via     \n");
  printf("   D2 |    0     1    3 \n");
  printf("  ----|-----------------\n");
  printf("     0|  %3d   %3d   %3d\n",dtptr->costs[0][0],
	 dtptr->costs[0][1],dtptr->costs[0][3]);
  printf("dest 1|  %3d   %3d   %3d\n",dtptr->costs[1][0],
	 dtptr->costs[1][1],dtptr->costs[1][3]);
  printf("     3|  %3d   %3d   %3d\n",dtptr->costs[3][0],
	 dtptr->costs[3][1],dtptr->costs[3][3]);
}
