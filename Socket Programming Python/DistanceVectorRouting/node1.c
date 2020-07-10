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

int connectcosts1[4] = { 1,  0,  1, 999 };

struct distance_table
{
  int costs[4][4];
} dt1;


/* students to write the following two routines, and maybe some others */


rtinit1()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			dt1.costs[i][j] = 999;

    dt1.costs[1][0] = 1;
	dt1.costs[1][1] = 0;
	dt1.costs[1][2] = 1;

	printdt1(&dt1);

	struct rtpkt pkt0, pkt2;
	pkt0.sourceid = 1;
	pkt0.destid = 0;
	pkt2.sourceid = 1;
	pkt2.destid = 2;

	for (int i = 0; i < 4; i++)
	{
		pkt0.mincost[i] = dt1.costs[1][i];
		pkt2.mincost[i] = dt1.costs[1][i];
	}

	tolayer2(pkt0);
	tolayer2(pkt2);
}


rtupdate1(rcvdpkt)
  struct rtpkt *rcvdpkt;

{
	if (rcvdpkt->destid != 1) return;

	int j = rcvdpkt->sourceid;
	int flag = 0;

	printf("\nPacket rcvd at node 1 from node %d \n", j);

	for (int i = 0; i < 4; i++)
	{
		if (dt1.costs[j][i] > dt1.costs[1][j] + rcvdpkt->mincost[i])
		{
			dt1.costs[j][i] = dt1.costs[1][j] + rcvdpkt->mincost[i];
			if(i != 1) flag = 1;
		}
	}
	if (flag == 1) //change in dt1 detected!
	{
		printf("\nDistance table at node 1 updated: \n");
		printdt1(&dt1);

		struct rtpkt pkt0, pkt2;
		pkt0.sourceid = 1;
		pkt0.destid = 0;
		pkt2.sourceid = 1;
		pkt2.destid = 2;
		pkt0.mincost[1] = 0;
		pkt2.mincost[1] = 0;

		for (int i = 0; i < 4; i++)
		{
			if (i == 1) continue;
			pkt0.mincost[i] = min(dt1.costs[0][i], dt1.costs[2][i], dt1.costs[3][i]);
			pkt2.mincost[i] = min(dt1.costs[0][i], dt1.costs[2][i], dt1.costs[3][i]);
		}

		printf("\nSending routing packets to nodes 0 and 2 with following mincost vector: \n");
		for (int i = 0; i < 4; i++)
			printf("%d\t", pkt0.mincost[i]);
		printf("\n");

		tolayer2(pkt0);
		tolayer2(pkt2);
	}
}


printdt1(dtptr)
  struct distance_table *dtptr;

{
  printf("             via   \n");
  printf("   D1 |    0     2 \n");
  printf("  ----|-----------\n");
  printf("     0|  %3d   %3d\n",dtptr->costs[0][0], dtptr->costs[0][2]);
  printf("dest 2|  %3d   %3d\n",dtptr->costs[2][0], dtptr->costs[2][2]);
  printf("     3|  %3d   %3d\n",dtptr->costs[3][0], dtptr->costs[3][2]);

}



linkhandler1(linkid, newcost)
int linkid, newcost;
/* called when cost from 1 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */

{
}

