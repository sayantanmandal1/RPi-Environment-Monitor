#!/bin/bash

SESSION_NAME="sensor_monitor"
VENV_PATH="/home/msaya/vibenet-env"
PROJECT_PATH="/home/msaya/test_serial"

# Kill existing session if running
tmux has-session -t $SESSION_NAME 2>/dev/null
if [ $? -eq 0 ]; then
  tmux kill-session -t $SESSION_NAME
fi

# Start new tmux session with serial_logger.py
tmux new-session -d -s $SESSION_NAME "source $VENV_PATH/bin/activate && python3 $PROJECT_PATH/serial_logger.py"

# Create vertical split for train_model.py
tmux split-window -v -t $SESSION_NAME:0
tmux send-keys -t $SESSION_NAME:0.1 "sleep 10 && source $VENV_PATH/bin/activate && python3 $PROJECT_PATH/train_model.py" C-m

# Create horizontal split for threshold_checker.py
tmux split-window -h -t $SESSION_NAME:0
tmux send-keys -t $SESSION_NAME:0.2 "sleep 30 && source $VENV_PATH/bin/activate && python3 $PROJECT_PATH/threshold_checker.py" C-m

# Layout and attach
tmux select-layout -t $SESSION_NAME tiled
tmux attach-session -t $SESSION_NAME

