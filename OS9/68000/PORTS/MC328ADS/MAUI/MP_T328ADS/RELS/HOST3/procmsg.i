���   �              �   x       8   oP                                                                                   procmsg.c                                                                                                                                                                                                                     �              �              �              �                                                       0             @             P             `             p                                          p             �          �         �             �             �  �  �      H                    mppm_process_msg    X  8  P             �                    (                     _$0 x  �  p                _$1 �  �  �                _$2 �  8  �           	   mppm        �  �  �      X        cmd_msg     h  �         x        reply_msg      8(8�  (8     �(��  (�X     �(��  (�x  �             �  �  �                       �         �             	   pmem        .   �(�h  $�"�X$X(8�  .X�   �  �                	   rmsg        6   �(��  �"�(��  �$�(�h  .�&       �"�(��  "�(��  x  P  0   
      " �$�(��  .�   (    " �$�(��  .� .  �                   �                err �  �        �        cmd_get_dev_cap     *                    (             W   "�$�(��  .�& (�  (��  8$8(��  �(�h  * (    " �$�(��  .� .  �  �  �      �        cmd_get_dev_status  n          �                       W   "�$�(��  .�j (�  (��  $(��  �(�h  * (    " �$�(��  .� .  �  �  �      �        cmd_set_ptr_pos                �         �             D      W   "�$�(��  .� (�  (��  �$�(��  �(�h  * (    " �$�(��  .� .  P	  �  �      8        cmd_set_sim_meth        �         �             W   "�$�(��  .� (�  (��  �$�(��  �(�h  * (    " �$�(��  .� .  (
  �  h	      �        cmd_set_ptr_limit   (          �	         �	             W   "�$�(��  .� (�  (�P	  �	$�	(��  �(�h  * (    " �$�(��  .� .    �  @
      �        cmd_set_msg_callback                  `
         h
             W   "�$�(��  .� (�  (�(
  x
$x
(��  �(�h  * (    " �$�(��  .� . 	 �  �         �"        cmd_set_msg_mask    W   "�$�(��  .� (�  (�  x
$x
(��  �(�h  * (    " �$�(��  .� .  �  �  �      $        cmd_reserve_key     8  �  �      %     W   "�$�(��  .� (�  (��  x
$x
(��  �(�h  * (    " �$�(��  .� .     cmd_release_key     W   "�$�(��  .� (�  (��  x
$x
(��  �(�h  * (    " �$�(��  .� .                            �    P      &        cmd_check_keys      K    " �$�(��  .�  (8  �$�(��  �(�h  *     2   "�$�(��  .� (�  +      (    " �$�(��  .� .  4   "�$�(��  .� (�  .
    T         +   
               '      #                                     (�      �  �  �             x  �     _$3    8  �                _$4 (  �        �     	   pmem        �  8  @      �     	   rmsg           �(�   (��     8(8(  (8�      �  �                       �         �                cap &   �(��  �$�(8(  .�      +X  �  �        (    @    �        H    h         �    H    4          �         4          x         4     
     X    �    4          (	    �	    4           
    �
    4          �
    8    4          �         4          X    �    4          �    h    �    4          �        4          �         " $�(��  .    &   � "� �$�(��  .� ,�   &   � "� �$�(��  .� +�   &    " �$�(��  .� +   &    " �$�(��  .�
 +   M     P    h    �    �    �         (    P    �     �  �  �                _$5                      _$6 0  �         �     	   pmem              H      �     	   rmsg           �(�  (��     (0  (�  �            �                 �  �                memcpy      R          �         �                +W   �(��  0.0R �$��$�(�  .�� �$��$�(0  .� * %     X    p         �     �  �  �             �  �     _$7 �  �  �                _$8 �  �  �      �     	   pmem        @  �        �        msg    �(��  (��     �(��  (��      �  X                status      &   �(�@  �$�(��  .��   �               �         �             6   � " $��$�(��  .� " $�(�@    6    " $��$�(��  .� " $�(�@    8  	   D   � " $��$�(��  .� " $��$�(�@  .�     D    " $��$�(��  .� " $��$�(�@  .�     D    " $��$�(�@  .� " $��$�(��  .�          <   � " �$�(��  .� " �$�(�@  .�     <    " �$�(��  .� " �$�(�@  .�     �     <   � " �$�(��  .� " �$�(�@  .�        +H  �  @             <    " �$�(��  .� " �$�(�@  .�     <    " �$�(�@  .� " �$�(��  .�     �         (    h    �    �    
          0    �    "          �        X         &     �         ,     (     @    �     8  �     _$9 p  �  `             	   _$10        �  �  �      �     	   pmem        �  �  �      H        msg    �(�p  (��     �(��  (�H      �                   status      &   �(��  �$�(�p  .��   H  x           " �$�(��  .�   
   .
                                W   �(��  0.0  �$��$�(��  .�$ �$��$�(��  .�* * <    " �$�(��  .� " �$�(��  .�        +u     �    �        8    0     2     4     
     6     h    .     �    �    0     �  �  �                 �  	   _$11          �	                   msg 	   _$12        8  �  (      �     	   pmem        �  �	  �      �        �(�  (��     �	(�	8  (�	�      �  �                status      &   �(��  �$�(�  .��   6    " $�(��  " $��$�(�	8  .�   D    " $��$�(��  .� " $��$�(�	8  .�      X     6   � " $��$�(��  .� " $�(��    6    " $��$�(��  .� " $�(��    �     D   � " $��$�(��  .� " $��$�(��  .�     D    " $��$�(��  .� " $��$�(��  .�     <    " �$�(��  .� " �$�(�	8  .�     <    " �$�(��  .� " �$�(�	8  .�$     �     <   � " �$�(��  .� " �$�(��  .�     <    " �$�(��  .� " �$�(��  .�     p     <   � " �$�(��  .� " �$�(��  .�     <    " �$�(��  .� " �$�(��  .�        +8"  �   �     P    h    �    �        P    �         :     �        @     <     `    �    �    (         D     h    �    J     F     �     �   �  �              	   _$13        !  x
  !             	   _$14        @!  �  0!      �      	   pmem        �!  x
  X!      �         msg    �(�!  (��      x
(x
@!  (x
�       �  �!                status      &   �(��!  �$�(�!  .��   �             <   �"��!$�!(��!  .�!N "��!$�!(x
@!  .�!        +-     `!    x!    �!    �!    0"     �"  �  �"             �#  h"  	   _$15        �"  x
  �"             	   _$16        �"  �  �"      h"     	   pmem        8#  x
   #      �"        msg    �(��"  (�h"     x
(x
�"  (x
�"      �  P#                status      &   �(�8#  �$�(��"  .��   <   "�$�(�8#  .�J "�$�(x
�"  .�        +-     #     #    `#    �#    �#     0$  �   $             �$   $  	   _$17        X$  x
  H$             	   _$18        �$  �  p$       $     	   pmem            x
  �$      0$        msg    �(�X$  (� $     x
(x
�$  (x
0$  
   .
           �$    �$    �$     0%  �   %             �%   %  	   _$19        X%  x
  H%             	   _$20        �%  �  p%       %     	   pmem            x
  �%      0%        msg    �(�X%  (� %     x
(x
�%  (x
0%  
   .
           �%    �%    �%     0&  �   &             �&   &  	   _$21        X&  �  H&             	   _$22        �&  �  p&       &     	   pmem            �  �&      0&        msg    �(�X&  (� &     �(��&  (�0&     +      �&    �&    �&                                                                                                                                                                                                                                                                             