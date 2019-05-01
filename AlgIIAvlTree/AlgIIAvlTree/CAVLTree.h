#pragma once

/*
	Code taken from http://www.cs.vsb.cz/dvorsky/Download/SkriptaAlgoritmy/Algoritmy.pdf
	Added minor comments
*/

template<class T> class CAVLTree
{
public:
	CAVLTree();
	~CAVLTree();
	void Insert(T x);
	void Delete(T x);
protected:
	struct CNode
	{
		T key;
		CNode* left;
		CNode* right;
		int bal; // NOTE: Tree balance -1, 0, 1
	}; // CNode;
	void FreeAll(CNode* p);
	void DoInsert(CNode*& p, T x, bool& h);
	void DoDelete(CNode*& p, T x, bool& h);
	void Balance1(CNode*& p, bool& h);
	void Balance2(CNode*& p, bool& h);
	void Del(CNode*& r, CNode*& q, bool& h);
	CNode* m_root;
}; // CAVLTree

template<class T> CAVLTree<T>::CAVLTree()
{
	m_root = nullptr;
}

template<class T> CAVLTree<T>::~CAVLTree()
{
	FreeAll(m_root);
}

template<class T> void CAVLTree<T>::Insert(T x)
{
	bool h = false;
	DoInsert(m_root, x, h);
} // CAVLTree::Insert

template<class T> void CAVLTree<T>::Delete(T x)
{
	bool h = false;
	DoDelete(m_root, x, h);
} // CAVLTree::Delete

template<class T> void CAVLTree<T>::DoInsert(CNode*& p, T x,
	bool& h)
{ // h == false

	/*
		NOTE: Description of parameters
		p - current node
		x - inserted value
		h - notifies caller, that there is possible imbalance down the line
		*p1, *p2 - helper variables used in rotations
	*/

	CNode *p1, *p2;

	// NOTE: Handle leaf case
	if (p == nullptr)
	{
		p = new CNode;
		p->key = x;
		p->left = p->right = nullptr;
		p->bal = 0;
		h = true;
		return;
	}; // if

	// NOTE: Recursively check left subtree
	if (x < p->key)
	{
		DoInsert(p->left, x, h);
		if (h)
		{
			// NOTE: Might be imbalanced
			switch (p->bal)
			{
			case 1:
				// NOTE: Was imbalanced to right, balance
				p->bal = 0;
				h = false;
				break;
			case 0:
				// NOTE: Tree was balanced, tilt to the left
				p->bal = -1;
				break;
			case -1:
				// NOTE: Tree was imbalanced to left, L{X} rotation needed
				p1 = p->left;
				if (p1->bal == -1)
				{// LL 
					p->left = p1->right;
					p1->right = p;
					p->bal = 0;
					p = p1;
				} // if
				else
				{// LR
					p2 = p1->right;
					p1->right = p2->left;
					p2->left = p1;
					p->left = p2->right;
					p2->right = p;
					p->bal = (p2->bal == -1) ? +1 : 0;
					p1->bal = (p2->bal == +1) ? -1 : 0;
					p = p2;
				}; // else
				p->bal = 0;
				h = false;
				break;
			}; // switch
		}; // if
		return;
	}; // if

	// NOTE: Recursively check right tree
	if (x > p->key)
	{
		DoInsert(p->right, x, h);
		if (h)
		{
			// NOTE: Might be imbalanced
			switch (p->bal)
			{
			case -1:
				// NOTE: Was imbalanced to left, balance
				p->bal = 0;

				h = false;
				break;
			case 0:
				// NOTE: Was balanced, tilt right
				p->bal = +1;
				break;
			case +1:
				// NOTE: Was tilted right, need R{X} rotation
				p1 = p->right;
				if (p1->bal == +1)
				{// RR
					p->right = p1->left;
					p1->left = p;
					p->bal = 0;
					p = p1;
				} // if
				else
				{// RL
					p2 = p1->left;
					p1->left = p2->right;
					p2->right = p1;
					p->right = p2->left;
					p2->left = p;
					p->bal = (p2->bal == +1) ? -1 : 0;
					p1->bal = (p2->bal == -1) ? +1 : 0;
					p = p2;
				}; // else
				p->bal = 0;
				h = false;
				break;
			}; // switch
		}; // if
		return;
	}; // if

	// duplicitni klic
	h = false;

} // CAVLTree::DoInsert

template<class T> void CAVLTree<T>::FreeAll(CNode* p)
{
	if (p != nullptr)
	{
		FreeAll(p->left); // zrušíme levý podstrom
		FreeAll(p->right); // zrušíme pravý podstrom
		delete p; // nakonec smažeme vrchol p
	}; // if
} // CAVLTree::FreeAll

template<class T> void CAVLTree<T>::DoDelete(CNode*& p, T x, bool& h)
{
	/*
		NOTE: Description of parameters
		p - current node
		x - key to delete
		h - notifies caller, that there is possible imbalance down the line
	*/

	if (p == nullptr)
		h = false; // klíè x není ve stromu
	else
		if (x < p->key)
		{
			// NOTE: Check left subtree
			DoDelete(p->left, x, h);
			if (h)
				Balance1(p, h);
		} // if
		else
			if (x > p->key)
			{
				// NOTE: Check right tree
				DoDelete(p->right, x, h);
				if (h)
					Balance2(p, h);
			} // if
			else
			{// x == p->key
				// NOTE: Found key, store node to temp variable q
				CNode *q = p;
				if (q->right == nullptr)
				{
					// NOTE: If there is only left child, use it instead of current node
					p = q->left;
					h = true;
				} // if
				else
					if (q->left == nullptr)
					{
						// NOTE: IF there is only right child, use it instead of current node
						p = q->right;
						h = true;
					} // if
					else
					{
						// NOTE: If there are both nodes, use closest key from left subtree
						Del(q->left, q, h);
						if (h)
							Balance1(p, h);
					}; // else

				// NOTE: Delete q in the end
				delete q;
			}; // else
} // CAVLTree::DoDelete

template<class T> void CAVLTree<T>::Balance1(CNode*& p, bool& h)
{// h = true, levá vìtev se zmenšila
	// NOTE: Fix balance after delete in left tree

	CNode *p1, *p2;
	int b1, b2;
	switch (p->bal)
	{
	case -1:
		// NOTE: Was imbalanced to left, balanced now
		p->bal = 0;
		break;
	case 0:
		// NOTE: Was balanced, imbalance to right
		p->bal = +1;
		h = false;
		break;
	case 1:
		// NOTE: Was imbalanced to right, has to rotate
		p1 = p->right;
		b1 = p1->bal;
		if (b1 >= 0)
		{// jednoducha RR rotace
			p->right = p1->left;
			p1->left = p;
			if (b1 == 0)
			{
				p->bal = +1;
				p1->bal = -1;
				h = false;
			} // if
			else
			{
				p->bal = 0;
				p1->bal = 0;
			}; // else
			p = p1;
		} // if
		else
		{// RL
			p2 = p1->left;
			b2 = p2->bal;
			p1->left = p2->right;
			p2->right = p1;
			p->right = p2->left;
			p2->left = p;
			p->bal = (b2 == +1) ? -1 : 0;
			p1->bal = (b2 == -1) ? +1 : 0;
			p = p2;
			p2->bal = 0;
		}; // else
		break;
	}; // switch
} // CAVLTree::Balance1

template<class T> void CAVLTree<T>::Balance2(CNode*& p, bool& h)
{// h = true, pravá vìtev se zmenšila

	// NOTE: Fix balance after delete in right tree
	CNode *p1, *p2;
	int b1, b2;
	switch (p->bal)
	{
	case 1:
		// NOTE: Was imbalanced to right, balanced now
		p->bal = 0;
		break;
	case 0:
		// NOTE: Was balanced, tilted to left
		p->bal = -1;
		h = false;
		break;
	case -1:
		// NOTE: Was already tilted left, has to rotate
		p1 = p->left;
		b1 = p1->bal;
		if (b1 == -1)
		{// LL
			p->left = p1->right;
			p1->right = p;
			if (b1 == 0)
			{
				p->bal = -1;
				p1->bal = +1;
				h = false;
			} // if
			else
			{
				p->bal = 0;
				p1->bal = 0;
			}; // else
			p = p1;
		} // if
		else
		{// LR
			p2 = p1->right;
			b2 = p2->bal;
			p1->right = p2->left;
			p2->left = p1;
			p->left = p2->right;
			p2->right = p;
			p->bal = (b2 == -1) ? +1 : 0;
			p1->bal = (b2 == +1) ? -1 : 0;
			p = p2;
			p2->bal = 0;
		}; // else
	}; // switch
} // CAVLTree::Balance2

template<class T> void CAVLTree<T>::Del(CNode*& r, CNode*& q,
	bool& h)
{// h = false
	if (r->right != nullptr)
	{
		Del(r->right, q, h);
		if (h)
			Balance2(r, h);
	} // if
	else
	{
		q->key = r->key;
		q = r;
		r = r->left;
		h = true;
	}; // else
} // CAVLTree::Del