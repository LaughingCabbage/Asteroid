#ifndef GAMESTATE_H
#define GAMESTATE_H


class GameState
{
	public:
		virtual void init() = 0;
		virtual void cleanup() = 0;
		virtual void pause() = 0;
		virtual void resume() = 0;
		virtual void handleEvents() = 0;
		virtual void update() = 0;
		virtual void draw() = 0;
		void changeState();


	protected:
		GameState(){}

	private:
};

#endif // GAMESTATE_H
