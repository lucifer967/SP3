#pragma once
class CAnimation
{
public:
	CAnimation();
	virtual ~CAnimation();

	// Set Animation status; left or right
	void SetAnimationStatus(bool m_bAnimationInvert, bool isMoving,bool isAttacking, bool isDied, double dt);
	// Update the Animation Index
	void UpdateAnimationIndex(double dt);
	// Get the Animation status
	bool GetAnimationStatus(void) const;
	// Get the Animation index
	int GetAnimationIndex(void) const;

	// Set right indices
	void SetRightIndices(const int m_iRight_Start, const int m_iRight_End);
	// Set left indices
	void SetLeftIndices(const int m_iLeft_Start, const int m_iLeft_End);

	// Set right idle indices
	void SetRightIdleIndices(const int m_iRightIdle_Start, const int m_iRightIdle_End);
	// Set left idle indices
	void SetLeftIdleIndices(const int m_iLeftIdle_Start, const int m_iLeftIdle_End);

	// Set right attack indices
	void SetRightAttIndices(const int m_iRightAtt_Start, const int m_iRightAtt_End);
	// Set left attack indices
	void SetLeftAttIndices(const int m_iLeftAtt_Start, const int m_iLeftAtt_End);

	// Set right attack indices
	void SetRightDiedIndices(const int m_iRightDied_Start, const int m_iRightDied_End);
	// Set left attack indices
	void SetLeftDiedIndices(const int m_iLeftDied_Start, const int m_iLeftDied_End);

private:
	// Flag to indicate if the Animation is inverted to the left. 
	// true==face right, false==face left
	bool m_bAnimationInvert;
	
	bool isMoving;

	bool isAttacking;
	bool isDied;

	double m_dElapsedTime;

	// 0 == The default hero frame, 1/2/3 == Right facing animations, 
	// 4/5/6 == Left facing animations
	int m_iAnimation_Index;

	// Start index of right facing Animations
	int m_iRight_Start;
	// End index of right facing Animations
	int m_iRight_End;
	// Start index of left facing Animations
	int m_iLeft_Start;
	// End index of left facing Animations
	int m_iLeft_End;

		// Start index of right facing Animations
	int m_iRightIdle_Start;
	// End index of right facing Animations
	int m_iRightIdle_End;
	// Start index of left facing Animations
	int m_iLeftIdle_Start;
	// End index of left facing Animations
	int m_iLeftIdle_End;

	// Start index of right facing Animations
	int m_iRightAtt_Start;
	// End index of right facing Animations
	int m_iRightAtt_End;
	// Start index of left facing Animations
	int m_iLeftAtt_Start;
	// End index of left facing Animations
	int m_iLeftAtt_End;

	// Start index of right facing Animations
	int m_iRightDied_Start;
	// End index of right facing Animations
	int m_iRightDied_End;
	// Start index of left facing Animations
	int m_iLeftDied_Start;
	// End index of left facing Animations
	int m_iLeftDied_End;
};

